#include "axel/ResourceManager.h"

#include <functional>
#include <iostream>
#include <memory>
#include <utility>

#include <spdlog/spdlog.h>

#include "axel/PlayerItems.h"
#include "axel/Prices.h"
#include "config/database.h"
#include "config/auth.h"
#include "config/poe_ninja_config.h"
#include "parse/json.h"
#include "parse/util.h"
#include "poe_ninja/get_poe_ninja_data.h"
#include "util/Database.h"
#include "util/HttpSender.h"
#include "util/type.h"

namespace json = boost::json;

namespace {
    /// @brief Retrieves the sort order of items given a sorting function.
    /// @param items List of items to sort through.
    /// @param Comparator function.
    /// @return A sorted list of unique item IDs.
    std::vector<int>
    get_sort_order(const std::vector<axel::Item>& items,
                   std::function<bool(const axel::Item& item_1,
                                      const axel::Item& item_2)> comp) {
        // Copy because sort mutates
        auto copy(items);
        std::sort(copy.begin(), copy.end(), comp);

        // Retrieve IDs in order
        std::vector<int> result{};
        for (auto item : copy) {
            result.push_back(item.get_id());
        }

        return result;
    }
}  // namespace

namespace axel {
    namespace ninja = config::poe_ninja;

    // Public
    ResourceManager::ResourceManager(const std::string& access_token,
                                     const std::string& league,
                                     const std::string& league_type) :
        access_token_{ access_token },
        league_{ league },
        league_type_{ league_type },
        player_items_{ std::move(access_token) },
        prices_{ std::make_unique<axel::Prices>(league, league_type) },
        database_{ std::make_unique<util::Database>("app") },
        http_sender_{ std::make_shared<util::HttpSender>() } {}

    std::string ResourceManager::get_update() {
        auto player_items = player_items_.get_update(ninja::leagues::standard);
        prices_->update_prices();

        std::vector<axel::Item> item_list{};
        int id = 0;  // Unique ID to give to each item
        for (auto& pair : player_items) {
            std::string name{ pair.first };
            int64_t quantity{ util::long_to_int64_t(pair.second) };
            double unit_price{ -1 };

            // Lookup corresponding unit price
            std::vector<std::string> attributes{ "name, unit_price" };
            auto prices_table = database_->select_row("prices",
                                                      name,
                                                      attributes);

            if (!prices_table.empty()) {
                auto unit_price_variant = prices_table.at("unit_price");
                // Get the double out of variant
                if (const double* unit_price_ptr =
                    std::get_if<double>(&unit_price_variant)) {
                    unit_price = *unit_price_ptr;
                }
            } else {
                spdlog::warn("Unit price not found for {}", name);
            }

            item_list
            .emplace_back(axel::Item{ id, name, quantity, unit_price });
            id++;
        }
        json::value item_list_json = serialise_items(item_list);
        return json::serialize(item_list_json);
    }

    // Protected
    ResourceManager::ResourceManager(std::string access_token,
                                     std::shared_ptr<util::IHttpSender>
                                     http_sender) :
        access_token_{ std::move(access_token) },
        http_sender_{ http_sender },
        player_items_{ access_token_ } {};

    json::value
    ResourceManager::serialise_items(const std::vector<axel::Item>& item_list) {
        json::string league{ league_ };
        json::string league_type{ league_type_ };
        json::array items{};
        // Populate items array
        for (auto& item : item_list) {
            json::value item_json{ { "name", item.get_name() },
                                   { "quantity", item.get_quantity() },
                                   { "unit-price", item.get_unit_price() },
                                   { "total-price", item.get_total_price() },
                                   { "graph", { 1, 2, 3, 4, 5 } } };

            items.emplace_back(std::move(item_json));
        }

        // Populate sort arrays
        auto sorted_by_name = get_sort_order(item_list,
                                             [](auto item_1, auto item_2) {
                                                 return (item_1.get_name() <
                                                         item_2.get_name());
                                             });

        auto sorted_by_quantity =
        get_sort_order(item_list, [](auto item_1, auto item_2) {
            return (item_1.get_quantity() < item_2.get_quantity());
        });

        auto sorted_by_unit_price =
        get_sort_order(item_list, [](auto item_1, auto item_2) {
            return (item_1.get_unit_price() < item_2.get_unit_price());
        });

        auto sorted_by_total_price =
        get_sort_order(item_list, [](auto item_1, auto item_2) {
            return (item_1.get_total_price() < item_2.get_total_price());
        });

        // Construct "sorted" property
        json::array name(sorted_by_name.begin(), sorted_by_name.end());
        json::array quantity(sorted_by_quantity.begin(),
                             sorted_by_quantity.end());
        json::array unit_price(sorted_by_unit_price.begin(),
                               sorted_by_unit_price.end());
        json::array total_price(sorted_by_total_price.begin(),
                                sorted_by_total_price.end());

        json::value sorted{ { "name", name },
                            { "quantity", quantity },
                            { "unit-price", unit_price },
                            { "total-price", total_price } };

        // Construct final JSON
        json::value result{ { "league", league },
                            { "league_type", league_type },
                            { "items", items },
                            { "sorted", sorted } };

        return result;
    }
}  // namespace axel
