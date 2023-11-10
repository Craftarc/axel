#ifndef AXEL_CONFIG_AUTH_H
#define AXEL_CONFIG_AUTH_H

#include <string>

namespace database {
    struct OAuthTable {
            std::string session_id;  // Primary key
            std::string state_hash;
            std::string code_challenge;
            std::string code_verifier;
            int time_to_live;
    };

    struct AppTable {
            std::string session_id;  // Primary key, Foreign key to oauth
            std::string access_token;
            int time_to_live;
    };

    struct PricesTable {
            std::string name;  // Primary key
            double unit_price;
    };

}  // namespace database

#endif
