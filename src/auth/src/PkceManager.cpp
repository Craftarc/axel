#include <string>
#include <botan/hash.h>
#include <botan/base64.h>

#include "auth/PkceManager.h"
#include "webutil/hash.h"

/// Generate and store a code verifier - code challenge pair.
auth::PkceManager::PkceManager() {
    // Get 32 random bytes
    std::vector<uint8_t> secret_bytes = webutil::generate_secret_bytes();
    
    // Convert to alphanumeric representation
    auto base64_verifier = Botan::base64_encode(secret_bytes.data(), secret_bytes.size());
    code_verifier_ = webutil::base64_url_encode(base64_verifier);
    
    
    // Hash bytes with SHA-256
    std::unique_ptr<Botan::HashFunction> sha_256{Botan::HashFunction::create(("SHA-256"))};
    sha_256->update(secret_bytes);
    auto hash = sha_256->final();
    
    // Convert to alphanumeric representation
    auto base64_challenge = Botan::base64_encode(hash.data(), hash.size());
    code_challenge_ = webutil::base64_url_encode(base64_challenge);
}

/// As per RFC 7636 Section 4, the code verifier is a cryptographic random string using the characters
/// [A-Z], [a-z], [0-9], "-", ".", "_", and "~", with a length between 43 and 128 characters.
/// @return std::string The code verifier string.
std::string auth::PkceManager::get_code_verifier() const {
    return code_verifier_;
}

/// The code challenge is derived from the code verifier. According to the PkceManager specification,
/// as well as GGG's requirements, the code challenge is hashed with SHA-256.
///
/// @return std::string The code challenge string.
std::string auth::PkceManager::get_code_challenge() const {
    return code_challenge_;
}


