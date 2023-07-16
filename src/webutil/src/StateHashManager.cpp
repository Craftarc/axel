#include <vector>
#include <botan/base64.h>

#include "webutil/StateHashManager.h"
#include "webutil/hash.h"

namespace {
    // Construct a base64url hash from 32 random bytes -> should be 43 characters
    std::string make_state_hash() {
        std::vector<uint8_t> secret_bytes = webutil::generate_secret_bytes();
        auto base64_state_hash = Botan::base64_encode(secret_bytes.data(), secret_bytes.size());
        return webutil::base64_url_encode(base64_state_hash);
    }
}

namespace webutil {
    StateHashManager::StateHashManager() : state_hash_(make_state_hash()) {}
    
    std::string StateHashManager::get_state_hash() const {
        return state_hash_;
    }
    
    bool StateHashManager::verify_state_hash(const std::string& check_hash) const {
        return (check_hash == state_hash_);
    }
}
