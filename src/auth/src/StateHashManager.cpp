#include "auth/StateHashManager.h"

#include <vector>

#include <aws/core/client/ClientConfiguration.h>
#include <botan/base64.h>

#include "util/hash.h"

namespace {
	// Construct a base64url hash from 32 random bytes -> should be 43 characters
	std::string make_state_hash() {
		std::vector<uint8_t> secret_bytes = util::generate_secret_bytes();
		auto base64_state_hash = Botan::base64_encode(secret_bytes.data(), secret_bytes.size());
		return util::base64_url_encode(base64_state_hash);
	}
}  // namespace

/// @return base64url state hash
std::string auth::StateHashManager::get_state_hash() const {
	return make_state_hash();
}

/// @param session_id The session ID of the user.
/// @param state_hash The state hash returned by the authorization server. This state hash is to be checked against
/// the state hash in the database associated with the session ID.
/// @return True if the state hashes match, False otherwise.
bool auth::StateHashManager::check_state_hash(std::string state_hash, std::string stored_hash) const {
	return state_hash == stored_hash;
}
