#ifndef AXEL_AXEL_RESOURCEMANAGER_H
#define AXEL_AXEL_RESOURCEMANAGER_H
#include <string>
#include <unordered_map>

#include <boost/beast/http.hpp>
#include <boost/json.hpp>

#include "Item.h"
#include "PlayerItems.h"
#include "axel/Prices.h"
#include "util/interfaces/IDatabase.h"
#include "util/interfaces/IHttpSender.h"
namespace http = boost::beast::http;

namespace axel {
    class ResourceManager {
            friend class ResourceManager_test;

        public:
            /// @brief Constructor
            explicit ResourceManager(const std::string& access_token,
                                     const std::string& league,
                                     const std::string& league_type);

            /// @brief Get an updated list of items in the player's stashes,
            /// and their cumulative value.
            /// @return JSON string of player items.
            std::string get_update();

        protected:
            /// @brief Constructor that allows for custom util::IHttpSender
            /// implementations.
            ResourceManager(std::string access_token,
                            std::shared_ptr<util::IHttpSender> http_sender);

        private:
            const std::string access_token_;
            const std::string league_;
            const std::string league_type_;
            std::shared_ptr<util::IHttpSender> http_sender_;
            axel::PlayerItems player_items_;
            std::unique_ptr<axel::IPrices> prices_;
            std::unique_ptr<util::IDatabase> database_;

            /// @brief Populates prices_table_ with pricing information
            /// Stores the information in prices_table_ as a (name, price) pair.
            /// @param prices String representation of a JSON response from PoE Ninja
            ///
            /// e.g. Price information for all currencies.
            void add_to_prices_table();

            /// @brief Converts player item information into JSON format
            /// Refer to docs/data-contracts/axel-client/detailed-net-worth-table
            /// @param List of player items.
            /// @return The output JSON.
            json::value
            serialise_items(const std::vector<axel::Item>& item_list);
    };
}  // namespace axel
#endif  // AXEL_AXEL_RESOURCEMANAGER_H
