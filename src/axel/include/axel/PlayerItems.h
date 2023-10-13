#ifndef AXEL_AXEL_PLAYERITEMS_H
#define AXEL_AXEL_PLAYERITEMS_H

#include <string>
#include <unordered_map>

#include "boost/beast/http.hpp"
#include "boost/json.hpp"
#include "util/interfaces/IHttpSender.h"

namespace http = boost::beast::http;
namespace json = boost::json;

namespace axel {
    /// @brief Manages information about the player's stashes' contents.
    class PlayerItems {
            friend class PlayerItems_test;

        public:
            /// @brief Constructor.
            explicit PlayerItems(const std::string& access_token);

            /// @brief Get updated information about the state of a player's inventory.
            const std::unordered_map<std::string, int64_t>&
            get_update(const std::string& league);

        protected:
            /// @brief Constructor. Allows the usage of a custom HttpSender.
            PlayerItems(const std::string& access_token,
                        std::shared_ptr<util::IHttpSender> http_sender);

        private:
            std::string access_token_;
            std::vector<std::string> stash_tab_ids_;
            std::unordered_map<std::string, int64_t> items_table_;
            std::shared_ptr<util::IHttpSender> http_sender_;
            /// @brief Prepares a Get Stash HTTP request to send to the PoE API
            http::request<http::string_body>
            make_get_stash_request(const std::string& id) const;

            /// @brief Populates items_table_ with information about the items_string in a player's inventory.
            void fill_items_table(const json::value& stash);

            /// @brief Fetches a list of the player's stashes for a particular
            /// league
            json::value list_stashes(const std::string& league) const;

            /// @brief Looks through a List Stashes response JSON for stash tab IDs and
            /// stores them
            void set_stash_tab_ids(boost::json::value list_stash_response);

            /// @brief Prepares a List Stashes HTTP request to send to the PoE API.
            http::request<http::string_body>
            make_list_stashes_request(const std::string& league) const;

            /// @brief Calls the List Stash endpoint, and get the response
            [[nodiscard]]
            boost::json::value get_stash(const std::string& id);
    };
}  // namespace axel
#endif  // AXEL_AXEL_PLAYERITEMS_H
