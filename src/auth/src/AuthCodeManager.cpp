#include "auth/AuthCodeManager.h"

#include "config/auth.h"
#include "config/axel.h"
#include "config/poe.h"
#include "util/path.h"

/// @param code_challenge The code challenge to include in the authorization url.
/// @param state_hash The state hash to include in the authorization url.
/// @return The authorization url string.
std::string auth::AuthCodeManager::get_auth_url(std::string code_challenge,
                                                std::string state_hash) const {
    namespace auth = config::auth;
    namespace scopes = auth::scopes;
#if AXEL_TEST
    std::string host = "http://" + auth::test_host;
    std::string redirect_uri = auth::paths::test_redirect_uri;
#else
    std::string host = "https://" + auth::host;
    std::string redirect_uri = auth::paths::redirect_uri;
#endif
    std::string base_path =
    host + auth::paths::auth_path;  // Before adding query parameters

    return util::add_query_parameters(
    base_path,
    { { "client_id", config::axel::client_id },
      { "response_type", "code" },
      { "scope",
        util::concatenate_with_space({ scopes::profile,
                                       scopes::characters,
                                       scopes::stashes,
                                       scopes::league_accounts,
                                       scopes::item_filter }) },
      { "state", state_hash },
      { "redirect_uri", redirect_uri },
      { "challenge", code_challenge },
      { "code_challenge_method", "S256" } });
}
