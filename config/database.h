#ifndef AXEL_CONFIG_POE_H
#define AXEL_CONFIG_POE_H

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

}  // namespace database

#endif
