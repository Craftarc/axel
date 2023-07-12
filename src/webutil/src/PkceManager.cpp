#include <string>
#include <algorithm>

#include <botan/auto_rng.h>
#include <botan/hash.h>
#include <botan/base64.h>

#include "webutil/PkceManager.h"

namespace webutil {
    PkceManager::PkceManager() {
        // Get 32 random bytes
        std::vector<uint8_t> secret_bytes = generate_secret_bytes();
        
        // Convert to alphanumeric representation
        auto base64_verifier = Botan::base64_encode(secret_bytes.data(), secret_bytes.size());
        code_verifier_ = base64_url_encode(base64_verifier);
        
        
        // Hash bytes with SHA-256
        std::unique_ptr<Botan::HashFunction> sha_256{Botan::HashFunction::create(("SHA-256"))};
        sha_256->update(secret_bytes);
        auto hash = sha_256->final();
        
        // Convert to alphanumeric representation
        auto base64_challenge = Botan::base64_encode(hash.data(), hash.size());
        code_challenge_ = base64_url_encode(base64_challenge);
        
    }
    
    std::string PkceManager::get_code_verifier() const {
        return code_verifier_;
    }
    
    std::string PkceManager::get_code_challenge() const {
        return code_challenge_;
    }
    
    std::vector<uint8_t> PkceManager::generate_secret_bytes() {
        const int VERIFIER_BYTE_LENGTH = 32;
        
        Botan::AutoSeeded_RNG rng;
        
        // Generate 32 random bytes
        std::vector<uint8_t> random_bytes(VERIFIER_BYTE_LENGTH);
        rng.randomize(random_bytes.data(), random_bytes.size());
        
        return random_bytes;
    }
    
    std::string& PkceManager::base64_url_encode(std::string& str) {
        // Replace characters
        std::replace(str.begin(), str.end(), '+', '-');
        std::replace(str.begin(), str.end(), '/', '_');
        
        // Move all '=' characters to the end, then deallocate that block of '=' characters
        str.erase(std::remove(str.begin(), str.end(), '='), str.end());
        
        return str;
    }
    
} // webutil

