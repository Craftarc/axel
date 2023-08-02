#ifndef AXEL_CONFIG_POE_AUTH_H
#define AXEL_CONFIG_POE_AUTH_H

namespace config {
    /// @brief Contains constants related to POE's API.
    namespace poe {
        const std::string host = "www.pathofexile.com";
        /// @brief Contains constants related to paths.
        namespace paths {
            const std::string auth_path = "/oauth/authorize";
            const std::string token = "/oauth/token";
            const std::string redirect_uri = "https://bibdsedr7muzq2hg5h2rqzs2ke0matoa.lambda-url.us-west-1.on.aws/";
        }
        /// @brief Contains constants related to data request scopes.
        namespace scopes {
            const std::string profile = "account:profile";
            const std::string stashes = "account:stashes";
            const std::string characters = "account:characters";
            const std::string league_accounts = "account:league_accounts";
            const std::string item_filter = "account:item_filter";
        }
    }
}

#endif // AXEL_CONFIG_POE_AUTH_H