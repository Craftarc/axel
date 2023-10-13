#ifndef AXEL_AXEL_RESOURCEMANAGER_H
#define AXEL_AXEL_RESOURCEMANAGER_H
#include <string>
#include <unordered_map>

#include <boost/beast/http.hpp>
#include <boost/json.hpp>

#include "Item.h"
#include "PlayerItems.h"
#include "util/interfaces/IHttpSender.h"
namespace http = boost::beast::http;

namespace axel {
    class ResourceManager {
            friend class ResourceManager_test;

        public:
            /// @brief Constructor
            explicit ResourceManager(const std::string& access_token);

            /// @brief Constructor
            explicit ResourceManager(std::string&& access_token);

            /// @brief Get an updated list of items in the player's stashes, and their cumulative value.
            std::vector<axel::Item> get_update();

        protected:
            /// @brief Constructor that allows for custom util::IHttpSender
            /// implementations.
            ResourceManager(std::string access_token, std::shared_ptr<util::IHttpSender> http_sender);

        private:
            const std::string access_token_;
            std::shared_ptr<util::IHttpSender> http_sender_;
            std::unordered_map<std::string, double> prices_table_;
            axel::PlayerItems player_items_;

            // Methods for getting price data from PoE Ninja
            /// @brief Calls a single PoE Ninja endpoint for all its prices, and stores
            /// the price information.
            void get_prices(const std::string& path, const std::string& league);

            /// @brief Populates prices_table_ with pricing information
            void add_to_prices_table(std::string& prices);

            /// @brief Looks up the unit price of a given item.
            [[nodiscard]]
            double find_in_prices_table(const std::string& item_name);
    };
}  // namespace axel
#endif  // AXEL_AXEL_RESOURCEMANAGER_H
