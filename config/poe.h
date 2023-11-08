#ifndef AXEL_CONFIG_POE_H
#define AXEL_CONFIG_POE_H

#include <string>

namespace config {
    /// @brief Contains constants related to POE's API.
    namespace poe {
        const std::string host = "https://www.pathofexile.com";
        const std::string api = "https://api.pathofexile.com";

        /// @brief Contains constants related to paths.
        namespace paths {
            const std::string auth_path = "/oauth/authorize";
            const std::string token = "/oauth/token";
            const std::string redirect_uri = "https://pathofaxel.com/auth/callback";
            const std::string list_stashes = "/stash/";
            const std::string get_stash = "/stash/standard/";
        }  // namespace paths

        /// @brief Contains constants related to data request scopes.
        namespace scopes {
            const std::string profile = "account:profile";
            const std::string stashes = "account:stashes";
            const std::string characters = "account:characters";
            const std::string league_accounts = "account:league_accounts";
            const std::string item_filter = "account:item_filter";
        }  // namespace scopes
    }      // namespace poe
}  // namespace config

#endif  // AXEL_CONFIG_POE_H
