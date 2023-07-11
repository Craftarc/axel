#include <string>

#include <botan/auto_rng.h>
#include <botan/hash.h>
#include <botan/hex.h>
#include <iostream>

#include "webutil/PkceManager.h"


namespace webutil {
    PKCE::PKCE() {
        Botan::AutoSeeded_RNG rng;
        
        // Generate 32 random bytes
        std::vector<uint8_t> random_bytes{32}; // TODO: Fix these magic numbers
        rng.randomize(random_bytes.data(), random_bytes.size());
        
        // Hash with SHA-256
        std::unique_ptr<Botan::HashFunction> sha_256{Botan::HashFunction::create(("SHA-256"))};
        sha_256->update(random_bytes);
        Botan::secure_vector<uint8_t> hash = sha_256->final();
        
        code_verifier_ = Botan::hex_encode(hash.data(), hash.size(), false);
    }
    
    std::string PKCE::get_code_verifier() const {
        return code_verifier_;
    }
}