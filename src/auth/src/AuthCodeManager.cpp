#include "auth/AuthCodeManager.h"

#include "config/axel.h"
#include "config/poe.h"
#include "util/path.h"

/// @param code_challenge The code challenge to include in the authorization url.
/// @param state_hash The state hash to include in the authorization url.
/// @return The authorization url string.
std::string auth::AuthCodeManager::get_auth_url(std::string code_challenge,
                                                std::string state_hash) const {
    std::string base_path =
    "https://" + config::poe::host +
    config::poe::paths::auth_path;  // Before adding query parameters
    namespace scopes = config::poe::scopes;

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
      { "redirect_uri", config::poe::paths::redirect_uri },
      { "challenge", code_challenge },
      { "code_challenge_method", "S256" } });
}
