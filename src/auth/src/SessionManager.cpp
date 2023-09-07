#include "auth/SessionManager.h"

#include <botan/base64.h>

#include "util/hash.h"

auth::SessionManager::SessionManager() {}

/// @return base64url-safe session token
std::string auth::SessionManager::get_session_token() const {
	// Construct base64, then encode to base64url
	auto random_bytes = util::generate_secret_bytes();
	auto token = Botan::base64_encode(random_bytes.data(), random_bytes.size());
	util::base64_url_encode(token);  // Make url safe
	return token;
}
