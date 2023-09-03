#ifndef AXEL_AXEL_RESOURCEMANAGER_H
#define AXEL_AXEL_RESOURCEMANAGER_H
#include <string>
#include <unordered_map>

#include <boost/beast/http.hpp>
#include <boost/json.hpp>

#include "webutil/interfaces/IHttpSender.h"
namespace http = boost::beast::http;

namespace axel {
	class ResourceManager {
		public:
			/// @brief Default constructor.
			explicit ResourceManager(std::string access_token);

			/// @brief Calls a single PoE Ninja endpoint for all its prices, and stores
			/// the price information.
			void get_prices(const std::string& path, const std::string& league);

			/// @brief Populates quantity_table_ with information about the items_string in a player's inventory.
			void fill_quantity_table(std::string& items_string);

		protected:
			/// @brief Constructor that allows for custom webutil::IHttpSender
			/// implementations.
			ResourceManager(std::string access_token, std::unique_ptr<webutil::IHttpSender> http_sender);

		private:
			const std::string access_token_;
			std::vector<std::string> stash_tab_ids_;
			std::unique_ptr<webutil::IHttpSender> http_sender_;
			std::unordered_map<std::string, int64_t> quantity_table_;
			std::unordered_map<std::string, double> prices_table_;

			/// @brief Populates prices_table_ with pricing information
			void fill_prices_table(std::string& prices);

			/// @brief Calls the GET /stash/<league> endpoint, and get the response.
			boost::json::value list_stashes();

			/// @brief Prepares a List Stashes HTTP request to send to the PoE API.
			[[nodiscard]]
			http::request<http::string_body> make_list_stashes_request() const;

			/// @brief Prepares a Get Stash HTTP request to send to the PoE API
			[[nodiscard]]
			http::request<http::string_body> make_get_stash_request(const std::string& id) const;

			/// @brief Looks through a List Stashes response JSON for stash tab IDs and
			/// stores them
			void set_stash_tab_ids(boost::json::value list_stash_response);

			/// @brief Calls the List Stash endpoint, and get the response
			[[nodiscard]]
			boost::json::value get_stash(const std::string& id);
	};
}  // namespace axel
#endif  // AXEL_AXEL_RESOURCEMANAGER_H