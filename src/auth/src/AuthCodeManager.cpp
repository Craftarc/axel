#include "auth/AuthCodeManager.h"

#include <spdlog/spdlog.h>

#include "axel/Config.h"
#include "util/path.h"

/// @param code_challenge The code challenge to include in the authorization url.
/// @param state_hash The state hash to include in the authorization url.
/// @return The authorization url string.
std::string auth::AuthCodeManager::get_auth_url(std::string code_challenge,
                                                std::string state_hash) const {
#ifdef AXEL_TEST
    std::string redirect_uri = "https://" + CONFIG("auth.host.axel.staging") +
                               CONFIG("auth.endpoint.redirect");
    ;
#else
    std::string redirect_uri = "https://" +
                               CONFIG("auth.host.axel.production") +
                               CONFIG("auth.endpoint.redirect");
#endif

    std::string base_path =
    CONFIG("auth.host.poe") +
    CONFIG("auth.endpoint.login");  // Before adding query parameters

    return util::add_query_parameters(
    base_path,
    { { "client_id", CONFIG("auth.client_id") },
      { "response_type", "code" },
      { "scope",
        util::concatenate_with_space({ CONFIG("auth.scopes.profile"),
                                       CONFIG("auth.scopes.characters"),
                                       CONFIG("auth.scopes.stashes"),
                                       CONFIG("auth.scopes.league_accounts"),
                                       CONFIG("auth.scopes.item_filter") }) },
      { "state", state_hash },
      { "redirect_uri", redirect_uri },
      { "challenge", code_challenge },
      { "code_challenge_method", "S256" } });
}
