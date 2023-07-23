#include <botan/base64.h>
#include "auth/SessionManager.h"
#include "webutil/hash.h"

auth::SessionManager::SessionManager() {}

std::string auth::SessionManager::get_session_token() const {
    auto random_bytes = webutil::generate_secret_bytes();
    auto token = Botan::base64_encode(random_bytes.data(), random_bytes.size());
    webutil::base64_url_encode(token); // Make url safe
    return token;
}

