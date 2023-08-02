#include <vector>
#include <botan/base64.h>
#include <aws/core/client/ClientConfiguration.h>

#include "auth/StateHashManager.h"
#include "webutil/hash.h"
#include "axel/Database.h"

namespace {
    // Construct a base64url hash from 32 random bytes -> should be 43 characters
    std::string make_state_hash() {
        std::vector<uint8_t> secret_bytes = webutil::generate_secret_bytes();
        auto base64_state_hash = Botan::base64_encode(secret_bytes.data(), secret_bytes.size());
        return webutil::base64_url_encode(base64_state_hash);
    }
}

std::string auth::StateHashManager::get_state_hash() const {
    return make_state_hash();
}

/// @param session_id The session ID of the user.
/// @param state_hash The state hash returned by the authorization server. This state hash is to be checked against
/// the state hash in the database associated with the session ID.
/// @return True if the state hashes match, False otherwise.
bool auth::StateHashManager::check_state_hash(std::string session_id, std::string state_hash) const {
    // Set up connection to Oauth Database
    Aws::Client::ClientConfiguration client_configuration;
    client_configuration.region = "us-west-1";
    axel::Database oauth_database{config::axel::database::auth, client_configuration};
    
    // Get stored state hash associated with session ID
    auto item_map = oauth_database.get(session_id);
    std::string stored_state_hash = item_map["state_hash"].GetS();
    
    return stored_state_hash == state_hash;
}





