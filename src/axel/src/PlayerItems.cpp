#include "axel/PlayerItems.h"

#include <cctype>
#include <regex>

#include "boost/json.hpp"
#include "config/poe.h"
#include "parse/json.h"
#include "parse/util.h"
#include "spdlog/spdlog.h"
#include "util/HttpSender.h"
#include "util/http.h"

namespace http = boost::beast::http;
namespace json = boost::json;

namespace {
    bool is_base64url(const std::string& url) {
        std::string pattern = "[ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789_-]{43}";  // URL safe

        std::regex base64url_regex(pattern);

        return std::regex_match(url, base64url_regex);
    }
}  // namespace

namespace axel {
    /// @brief Constructor. Allows the use of a custom HttpSender.
    /// @param access_token The access token that PlayerItems will use to make queries to GGG.
    /// @param http_sender The HttpSender to use.
    /// @see util::HttpSender
    /// @note access_token_ is a const reference. PlayerItems is a member of ResourceManager. Do not instantiate independently.
    PlayerItems::PlayerItems(const std::string& access_token, std::shared_ptr<util::IHttpSender> http_sender) :
        init_{ sanitise(access_token) },
        access_token_{ access_token },
        stash_tab_ids_{},
        items_table_{},
        http_sender_{ std::move(http_sender) } {}

    // Public
    /// @return A read-only view of a table containing information about a players' stashes.
    const std::unordered_map<std::string, int64_t>& PlayerItems::get_update() {
        json::value stashes(list_stashes());
        set_stash_tab_ids(stashes);
        fill_items_table();
        return items_table_;
    }

    // Private
    /// Stash tab IDs are 10-digit hexadecmial numbers.
    /// Stores stash tab IDs in stash_tab_ids_
    /// Reference: https://www.pathofexile.com/developer/docs/reference#type-StashTab
    /// @param list_stash_response boost::json::value response from a List Stashes API call.
    /// @note Directly modifiers the private stash_tab_ids_ vector.
    void PlayerItems::set_stash_tab_ids(boost::json::value list_stash_response) {
        // Try to look for value at /stashes
        boost::system::error_code ec;
        json::value* stashes(list_stash_response.find_pointer("/stashes", ec));
        if (ec.value() != 0) {
            spdlog::error("PlayerItems::set_stash_tab_ids - Error finding value at '/stashes'");
            std::exit(1);
        }
        // 'stashes' value should be an array
        json::array* stashes_list{ stashes->if_array() };
        if (stashes_list == nullptr) {
            spdlog::error("PlayerItems::set_stash_tab_ids - Value at '/stashes' is not a JSON array");
        }
        for (auto& item : *stashes_list) {
            json::value* name{ item.find_pointer("/id", ec) };
            if (ec.value() != 0) {
                spdlog::error("PlayerItems::set_stash_tab_ids - Error finding value at '/id' in a stash object");
                std::exit(1);
            }
            stash_tab_ids_.emplace_back(name->as_string());
        }
    }

    /// Stores the information in items_table_ as a (item-name, quantity) pair.
    /// Quantity represents the number of that item in the player's stashes.
    void PlayerItems::fill_items_table() {
        json::value stashes(list_stashes());
        // Temporary counter to limit number of stashes queried to 10
        int counter{ 0 };
        set_stash_tab_ids(stashes);
        for (auto& id : stash_tab_ids_) {
            if (counter >= 10) {  // TODO: Remove this and implement a proper stash selection system
                break;
            }
            json::value items_json(get_stash(id));

            // Get the array at 'items_string'
            json::array items(parse::get<json::array>(items_json, "/stash/items").get());

            // Iterate through each item
            for (auto& item : items) {
                // Find the item name
                json::string raw_item_name{ parse::get<json::string>(item, "/typeLine").get() };
                std::string item_name{ parse::to_kebab(raw_item_name.c_str()) };

                // Find the quantity of the item present
                int64_t quantity{ parse::get<int64_t>(item, "/stackSize").get() };

                // Store in table
                if (auto iterator{ items_table_.find(item_name) };
                    iterator != items_table_.end()) {  // Item name is found

                    iterator->second += quantity;
                } else {
                    items_table_.emplace(std::move(item_name), quantity);
                }
            }
            counter += 1;
        }
    }

    /// @return The JSON response body.
    json::value PlayerItems::list_stashes() {
        auto request{ make_list_stashes_request() };
        std::string response{ http_sender_->send_http_request(request) };
        return json::parse(response);
    }

    /// @return The HTTP request body.
    http::request<http::string_body> PlayerItems::make_list_stashes_request() const {
        const std::unordered_map<std::string, std::string> fields{ { "authorization", "Bearer " + access_token_ },
                                                                   { "host", config::poe::api } };
        return util::make_http_request("GET", config::poe::paths::list_stashes, fields);
    }

    /// @param The 10-digit hexadecimal ID of the stash to get information about.
    /// @return The JSON data of the items in the specified stash.
    boost::json::value PlayerItems::get_stash(const std::string& id) {
        auto request{ make_get_stash_request(id) };
        auto response{ http_sender_->send_http_request(request) };
        return json::parse(response);
    }

    /// The request gets information about a specific stash tab in the player's stash.
    /// Reference: https://www.pathofexile.com/developer/docs/reference#stashes-get
    /// @param id The id of the stash to get information about.
    /// @return The Get Stash HTTP request.
    http::request<http::string_body> PlayerItems::make_get_stash_request(const std::string& id) const {
        const std::unordered_map<std::string, std::string> fields{ { "authorization", "Bearer " + access_token_ },
                                                                   { "host", config::poe::api } };
        // Using the GET /stash/<league>/<stash_id> endpoint
        std::string path{ config::poe::paths::get_stash + id };
        return util::make_http_request("GET", path, fields);
    }

    /// Constructs PlayerItems using the default HttpSender implementation.
    /// @param access_token The access token this PlayerItems will query GGG servers for stash information with.
    PlayerItems::PlayerItems(const std::string& access_token) :
        PlayerItems{ access_token, std::make_shared<util::HttpSender>() } {}

    int PlayerItems::sanitise(const std::string& access_token) {
        // access_token
        // must be 43 characters
        if (access_token.length() != 43) {
            throw std::runtime_error("access_token is not 43 characters");
        }
        // must be base64url safe
        if (!is_base64url(access_token)) {
            throw std::runtime_error("access_token is not base64url encoded");
        }
        return 0;
    }
}  // namespace axel