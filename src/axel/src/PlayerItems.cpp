#include "axel/PlayerItems.h"

#include <cctype>
#include <regex>

#include "axel/Config.h"
#include "parse/json.h"
#include "parse/util.h"
#include "spdlog/spdlog.h"
#include "util/HttpSender.h"
#include "util/http.h"

namespace http = boost::beast::http;
namespace json = boost::json;

namespace {
    bool is_base64url(const std::string& url) {
        std::string pattern = "[ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstu"
                              "vwxyz0123456789_-]{43}";  // URL safe

        std::regex base64url_regex(pattern);

        return std::regex_match(url, base64url_regex);
    }
}  // namespace

namespace axel {
    /// @brief Constructor. Allows the use of a custom HttpSender.
    /// @param access_token The access token that PlayerItems will use to make
    /// queries to GGG.
    /// @param http_sender The HttpSender to use.
    /// @see util::HttpSender
    /// @note access_token_ is a const reference. PlayerItems is a member of
    /// ResourceManager. Do not instantiate independently.
    PlayerItems::PlayerItems(const std::string& access_token,
                             std::shared_ptr<util::IHttpSender> http_sender) :
        access_token_{ access_token },
        stash_tab_ids_{},
        items_table_{},
        http_sender_{ std::move(http_sender) } {}

    // Public
    /// @param league The league to fetch player stash information from.
    /// @return A read-only view of a table containing information about a players' stashes.
    const std::unordered_map<std::string, int64_t>&
    PlayerItems::get_update(const std::string& league) {
        json::value stashes(list_stashes(league));
        if (set_stash_tab_ids(stashes)) {
            // For now, get the first 3 stashes
            std::vector<std::string> target_stash_ids{ stash_tab_ids_[0],
                                                       stash_tab_ids_[1],
                                                       stash_tab_ids_[2] };
            for (auto& id : target_stash_ids) {
                json::value stash = get_stash(id);
                fill_items_table(stash);
            }
        }
        return items_table_;
    }

    // Private
    /// Stash tab IDs are 10-digit hexadecmial numbers.
    /// Stores stash tab IDs in stash_tab_ids_
    /// Reference: https://www.pathofexile.com/developer/docs/reference#type-StashTab
    /// @param list_stash_response boost::json::value response from a List Stashes API call.
    /// @return True if successful.
    /// @note Directly modifiers the private stash_tab_ids_ vector.
    bool
    PlayerItems::set_stash_tab_ids(boost::json::value list_stash_response) {
        // Try to look for value at /stashes
        boost::system::error_code ec;
        json::value* stashes(list_stash_response.find_pointer("/stashes", ec));
        if (ec.value() != 0) {
            spdlog::error("Error finding value at '/stashes'");
            return false;
        }
        // 'stashes' value should be an array
        json::array* stashes_list{ stashes->if_array() };
        if (stashes_list == nullptr) {
            spdlog::error("Value at '/stashes' is not a JSON array");
            return false;
        }
        for (auto& item : *stashes_list) {
            json::value* name{ item.find_pointer("/id", ec) };
            if (ec.value() != 0) {
                spdlog::error(
                "Could not find value at '/id' in a stash object");
                return false;
            }
            stash_tab_ids_.emplace_back(name->as_string());
        }

        return true;
    }

    /// Stores the information in items_table_ as a (item-name, quantity) pair.
    /// Quantity represents the number of that item in the player's stashes.
    /// @param stash The json string of the stash to parse for items
    void PlayerItems::fill_items_table(const json::value& stash) {
        // Get the array of items at 'items_string'
        json::array items(parse::get<json::array>(stash, "/stash/items").get());

        // Iterate through each item
        for (auto& item : items) {
            // Find the item name
            json::string raw_item_name{
                parse::get<json::string>(item, "/typeLine").get()
            };
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
    }

    /// @param league The league to fetch player stash information from
    /// @return The JSON response body.
    json::value PlayerItems::list_stashes(const std::string& league) const {
        auto request{ make_list_stashes_request(league) };
        std::string response{ http_sender_->send_http_request(request) };
        return json::parse(response);
    }

    /// @return The HTTP request body.
    http::request<http::string_body>
    PlayerItems::make_list_stashes_request(const std::string& league) const {
        const std::unordered_map<std::string, std::string> fields{
            { "authorization", "Bearer " + access_token_ },
            { "host", CONFIG("api.host.poe") }
        };
        return util::make_http_request("GET",
                                       CONFIG("api.poe.endpoint.list_stashes") +
                                       league,
                                       fields);
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
    http::request<http::string_body>
    PlayerItems::make_get_stash_request(const std::string& id) const {
        const std::unordered_map<std::string, std::string> fields{
            { "authorization", "Bearer " + access_token_ },
            { "host", CONFIG("api.host.poe") }
        };
        // Using the GET /stash/<league>/<stash_id> endpoint
        std::string path{ CONFIG("api.endpoint.get_stash") + id };
        return util::make_http_request("GET", path, fields);
    }

    /// Constructs PlayerItems using the default HttpSender implementation.
    /// @param access_token The access token this PlayerItems will query GGG servers for stash information with.
    PlayerItems::PlayerItems(const std::string& access_token) :
        PlayerItems{ access_token, std::make_shared<util::HttpSender>() } {}
}  // namespace axel
