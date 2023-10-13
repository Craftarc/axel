#include "axel/ResourceManager.h"

#include <iostream>
#include <utility>

#include <spdlog/spdlog.h>

#include "axel/PlayerItems.h"
#include "config/poe.h"
#include "config/poe_ninja_config.h"
#include "parse/json.h"
#include "parse/util.h"
#include "poe_ninja/get_poe_ninja_data.h"
#include "util/HttpSender.h"
#include "util/http.h"

namespace json = boost::json;

namespace axel {
    // Public
    ResourceManager::ResourceManager(const std::string& access_token) :
        access_token_{ access_token },
        player_items_{ access_token } {}

    ResourceManager::ResourceManager(std::string&& access_token) :
        access_token_{ std::move(access_token) },
        player_items_{ access_token_ } {}

    /// @return A list of items in the player's stashes
    std::vector<axel::Item> ResourceManager::get_update() {
            return std::vector<axel::Item>{};
    }

    // Protected
    ResourceManager::ResourceManager(std::string access_token, std::shared_ptr<util::IHttpSender> http_sender) :
        access_token_{ std::move(access_token) },
        http_sender_{ http_sender },
        player_items_{ access_token_ } {};

    /// Stores the information in prices_table_ as a (name, price) pair.
    /// @param prices String representation of a JSON response from PoE Ninja
    ///
    /// e.g. Price information for all currencies.
    void ResourceManager::add_to_prices_table(std::string& prices) {
        json::value prices_value(json::parse(prices));
        parse::JsonResult<json::array> prices_result{ parse::get<json::array>(prices_value, "/lines") };
        // Check if the 'lines' key was found and was an array
        json::array prices_array{};
        if (prices_result.is_success()) {
            prices_array = prices_result.get();
        } else {
            prices_array = {};
        }
        // Iterate through all the items in the array
        for (auto& item : prices_array) {
            // Name value could be keyed by /currencyTypeName or /name
            parse::JsonResult<json::string> name_result{ parse::get<json::string>(item, "/currencyTypeName") };
            json::string name{};
            double price{};
            if (name_result.is_success()) {  // Name value is keyed by /currencyTypeName
                name = name_result.get().c_str();
                // Value key for currencyOverview pricelists
                price = 1 / (parse::get<double>(item, "/pay/value").get());
            } else {  // Name value is keyed by /name
                std::string prices_kebab{ parse::to_kebab(prices) };
                name = parse::get<json::string>(item, "/name")
                       .get()
                       .c_str();  // TODO: Probably want to handle this failing as well
                price = 1 / (parse::get<double>(item, "/chaosValue").get());  // Value key for itemOverview pricelists
            }
            std::string name_kebab{ parse::to_kebab(name.c_str()) };
            // Put into prices table
            prices_table_.emplace(std::move(name_kebab), price);
        }
    }

    /// @param path
    /// Stores the information in prices_table_ as an (item-name, price) pair.
    /// @note Only retrieves prices for one endpoint. For example, calling the
    /// currency endpoint will retrieve all price information provided by that .
    /// endpoint only.
    void ResourceManager::get_prices(const std::string& path, const std::string& league) {
        // Get to the 'lines' array, where all the price information is
        std::string full_path{ path + "&league=" + league };
        std::string prices{ poe_ninja::get_item_prices(full_path, http_sender_) };
        add_to_prices_table(prices);
    }

    /// @param item_name Item name key to look for in prices_table_.
    /// @return 0 if the item was not found.
    /// @note Will log that the item name was not found.
    double ResourceManager::find_in_prices_table(const std::string& item_name) {
        if (prices_table_.find(item_name) != prices_table_.end()) {  // Item name was found in prices_table_
            return prices_table_.at(item_name);
        } else {
            spdlog::warn("Item name '{}' not found in prices_table_", item_name);
            return 0;
        }
    }
}  // namespace axel
