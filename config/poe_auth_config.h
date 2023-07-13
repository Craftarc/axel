#ifndef AXEL_CONFIG_POE_AUTH_CONFIG_H
#define AXEL_CONFIG_POE_AUTH_CONFIG_H
/**
 * @brief Configuration constants for use throughout the program.
 *
 * All strings are read-only.
 */
namespace config {
    namespace poe {
        const std::string host = "www.pathofexile.com";
        namespace paths {
            const std::string auth_path = "/oauth/authorize";
            const std::string redirect_uri = "https://bibdsedr7muzq2hg5h2rqzs2ke0matoa.lambda-url.us-west-1.on.aws/";
        }
        namespace scopes {
            namespace account {
                const std::string profile = "account:profile";
                const std::string stashes = "account:stashes";
                const std::string characters = "account:characters";
                const std::string league_accounts = "account:league_accounts";
                const std::string item_filter = "account:item_filter";
            }
        }
    }
}

#endif