#ifndef AXEL_AXEL_PRICES_H
#define AXEL_AXEL_PRICES_H

#include <memory>
#include <string>
#include <unordered_map>

#include "interface/IPrices.h"
#include "util/Database.h"
#include "util/interfaces/IHttpSender.h"

namespace axel {
    class Prices : public IPrices {
        friend class Prices_test;
        public:
            Prices(const std::string& league, const std::string& league_type);

            std::unordered_map<std::string, double>
            get_prices(const std::string& type_path) override;

            int update_prices() override;

        protected:
            Prices(const std::string& league,
                   const std::string& league_type,
                   std::shared_ptr<util::IHttpSender> http_sender,
                   std::unique_ptr<util::IDatabase> database);

        private:
            const std::string league_;
            const std::string league_type_;
            std::string type_;
            std::shared_ptr<util::IHttpSender> http_sender_;
            std::unique_ptr<util::IDatabase> database_;

            /// @brief Parses a JSON string from a call to a POE Ninja endpoint
            /// to get the useful pricing information out of it.
            /// @param Pricing response from POE Ninja endpoint.
            /// @return Hash table of item names and their corresponding unit prices.
            std::unordered_map<std::string, double>
            parse_prices(const std::string& prices);
    };
}  // namespace axel

#endif
