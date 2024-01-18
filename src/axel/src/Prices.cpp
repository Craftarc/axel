#include "axel/Prices.h"

#include <unordered_map>

#include <parse/util.h>

#include "axel/Config.h"
#include "boost/json.hpp"
#include "config/poe_ninja_config.h"
#include "parse/JsonResult.h"
#include "parse/json.h"
#include "util/HttpSender.h"
#include "util/http.h"
#include "util/path.h"

namespace axel {
    // Public
    Prices::Prices(const std::string& league, const std::string& league_type) :
        Prices{ league,
                league_type,
                std::make_shared<util::HttpSender>(),
                std::make_unique<util::Database>("app") } {};

    int Prices::update_prices() {
        auto price_table = get_prices(CONFIG("api.ninja.endpoint.currency"));
        // Put into prices database
        for (auto& price : price_table) {
            std::string name{ price.first };
            double unit_price{ price.second };
            database::PricesTable row{ name, unit_price };
            database_->insert_row("prices", row);
        }

        return 1;
    }

    std::unordered_map<std::string, double>
    Prices::get_prices(const std::string& type_path) {
        using Request = util::http::request<util::http::string_body>;

        // Construct full path to query
        std::unordered_map<std::string, std::string> parameters{
            { "league", league_type_ + league_ }
        };
        std::string path{ util::add_query_parameters(type_path, parameters) };

        Request request =
        util::make_http_request("GET",
                                path,
                                { { "host", CONFIG("api.host.ninja") } });

        std::string response =
        http_sender_->send_http_request(request,
                                        15);  // TODO: Remove magic number
        return parse_prices(response);
    }

    // Protected
    Prices::Prices(const std::string& league,
                   const std::string& league_type,
                   std::shared_ptr<util::IHttpSender> http_sender,
                   std::unique_ptr<util::IDatabase> database) :
        league_{ league },
        league_type_{ league_type },
        type_{},
        http_sender_{ http_sender },
        database_{ std::move(database) } {};

    // Private
    std::unordered_map<std::string, double>
    Prices::parse_prices(const std::string& prices) {
        namespace json = boost::json;
        std::unordered_map<std::string, double> result{};
        json::value prices_json;
        try {
            prices_json = json::parse(prices);
        } catch (const std::runtime_error& e) {
            spdlog::error("{}", e.what());
        }
        auto prices_result = parse::get<json::array>(prices_json, "/lines");
        // check if the 'lines' key was found and was an array
        json::array prices_array{};
        if (prices_result.is_success()) {
            prices_array = prices_result.get();
            spdlog::debug("Prices array size: {}", prices_array.size());
        } else {
            spdlog::warn("'lines' key does not exist in JSON");
            prices_array = {};
        }
        // iterate through all the items in the array
        for (auto& item : prices_array) {
            // name value could be keyed by /currencyTypeName or /name
            parse::JsonResult<json::string> name_result{
                parse::get<json::string>(item, "/currencyTypeName")
            };
            json::string name{};
            double price{};
            if (name_result
                .is_success()) {  // name value is keyed by /currencyTypeName
                name = name_result.get().c_str();
                // value key for currencyoverview pricelists
                price = 1 / (parse::get<double>(item, "/pay/value").get());
            } else {  // name value is keyed by /name
                std::string prices_kebab{ parse::to_kebab(prices) };
                name =
                parse::get<json::string>(item, "/name")
                .get()
                .c_str();  // todo: probably want to handle this failing as well
                price = 1 / (parse::get<double>(item, "/chaosValue")
                             .get());  // value key for itemoverview pricelists
            }
            std::string name_kebab{ parse::to_kebab(name.c_str()) };
            result.insert({ name_kebab, price });
        }

        return result;
    }
}  // namespace axel
