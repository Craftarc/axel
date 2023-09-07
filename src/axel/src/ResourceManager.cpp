#include "axel/ResourceManager.h"

#include <iostream>

#include <spdlog/spdlog.h>

#include "config/poe.h"
#include "config/poe_ninja_config.h"
#include "parse/json.h"
#include "parse/util.h"
#include "poe_ninja/get_poe_ninja_data.h"
#include "util/HttpSender.h"
#include "util/http.h"

namespace http = boost::beast::http;
namespace json = boost::json;

namespace axel {
	// Public
	/// Uses the real implementation of HttpSender
	/// @param access_token: Access token to use for making calls to the PoE API.
	ResourceManager::ResourceManager(std::string access_token) :
		ResourceManager{ std::move(access_token), std::make_unique<webutil::HttpSender>() } {};

	// Protected
	ResourceManager::ResourceManager(std::string access_token, std::unique_ptr<webutil::IHttpSender> http_sender) :
		access_token_{ std::move(access_token) },
		stash_tab_ids_{ std::vector<std::string>() },
		http_sender_{ std::move(http_sender) } {}

	// Private
	/// Stash tab IDs are 10-digit hexadecmial numbers.
	/// Stores stash tab IDs in stash_tab_ids_
	/// Reference: https://www.pathofexile.com/developer/docs/reference#type-StashTab
	/// @param list_stash_response boost::json::value response from a List Stashes API call.
	/// @note Directly modifiers the private stash_tab_ids_ vector.
	void ResourceManager::set_stash_tab_ids(boost::json::value list_stash_response) {
		// Try to look for value at /stashes
		boost::system::error_code ec;
		json::value* stashes(list_stash_response.find_pointer("/stashes", ec));
		if (ec.value() != 0) {
			spdlog::error("ResourceManager::set_stash_tab_ids - Error finding value at '/stashes'");
			std::exit(1);
		}
		// 'stashes' value should be an array
		json::array* stashes_list{ stashes->if_array() };
		if (stashes_list == nullptr) {
			spdlog::error("ResourceManager::set_stash_tab_ids - Value at '/stashes' is not a JSON array");
		}
		for (auto& item : *stashes_list) {
			json::value* name{ item.find_pointer("/id", ec) };
			if (ec.value() != 0) {
				spdlog::error("ResourceManager::set_stash_tab_ids - Error finding value at '/id' in a stash object");
				std::exit(1);
			}
			stash_tab_ids_.emplace_back(name->as_string());
		}
	}

	/// @return The HTTP request body.
	http::request<http::string_body> ResourceManager::make_list_stashes_request() const {
		const std::unordered_map<std::string, std::string> fields{ { "authorization", "Bearer " + access_token_ },
			                                                       { "host", config::poe::api } };
		return util::make_http_request("GET", config::poe::paths::list_stashes, fields);
	}

	/// The request gets information about a specific stash tab in the player's stash.
	/// Reference: https://www.pathofexile.com/developer/docs/reference#stashes-get
	/// @param id The id of the stash to get information about.
	/// @return The Get Stash HTTP request.
	http::request<http::string_body> ResourceManager::make_get_stash_request(const std::string& id) const {
		const std::unordered_map<std::string, std::string> fields{ { "authorization", "Bearer " + access_token_ },
			                                                       { "host", config::poe::api } };
		// Using the GET /stash/<league>/<stash_id> endpoint
		std::string path{ config::poe::paths::get_stash + id };
		return util::make_http_request("GET", path, fields);
	}

	/// @return The JSON response body.
	json::value ResourceManager::list_stashes() {
		auto request{ make_list_stashes_request() };
		std::string response{ http_sender_->send_http_request(request) };
		return json::parse(response);
	}

	/// @param The 10-digit hexadecimal ID of the stash to get information about.
	/// @return The JSON data of the items in the specified stash.
	boost::json::value ResourceManager::get_stash(const std::string& id) {
		auto request{ make_get_stash_request(id) };
		auto response{ http_sender_->send_http_request(request) };
		return json::parse(response);
	}

	/// Stores the information in prices_table_ as a (name, price) pair.
	/// @param prices String representation of a JSON response from PoE Ninja containing price information.
	void ResourceManager::fill_prices_table(std::string& prices) {
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
				name = parse::get<json::string>(item, "/name")
				       .get()
				       .c_str();  // TODO: Probably want to handle this failing as well
				price = 1 / (parse::get<double>(item, "/chaosValue").get());  // Value key for itemOverview pricelists
			}
			// Put into prices table
			prices_table_.emplace(std::move(name), price);
		}
	}

	/// @param path
	/// Stores the information in prices_table_ as an (item-name, price) pair.
	/// @note Only retrieves prices for one endpoint. For example, calling the currency endpoint will retrieve
	/// all price information provided by that endpoint only.
	void ResourceManager::get_prices(const std::string& path, const std::string& league) {
		// Get to the 'lines' array, where all the price information is
		std::string full_path{ path + "&league=" + league };
		std::string prices{ poe_ninja::get_item_prices(full_path) };
		fill_prices_table(prices);
	}

	/// @param items_string String representation of a JSON response from GGG containing player stashes information.
	/// Stores the information in quantity_table_ as a (item-name, quantity) pair.
	/// Quantity represents the number of that item in the player's stashes.
	void ResourceManager::fill_quantity_table(std::string& items_string) {
		json::value stashes(json::parse(items_string));
		set_stash_tab_ids(stashes);
		for (auto& id : stash_tab_ids_) {
			spdlog::trace("stash id: {}", id);
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
				if (auto iterator{ quantity_table_.find(item_name) };
				    iterator != quantity_table_.end()) {  // Item name is found

					iterator->second += quantity;
				} else {
					quantity_table_.emplace(std::move(item_name), quantity);
				}
			}
			break;
		}

		for (auto& pair : quantity_table_) {
			spdlog::trace("Name: {}\nQuantity: {}\n\n", pair.first, pair.second);
		}
	}
}  // namespace axel