#include <botan/auto_rng.h>
#include <algorithm>

#include "webutil/hash.h"

namespace webutil {
    std::vector<uint8_t> generate_secret_bytes() {
        const int VERIFIER_BYTE_LENGTH = 32;
        
        Botan::AutoSeeded_RNG rng;
        
        // Generate 32 random bytes
        std::vector<uint8_t> random_bytes(VERIFIER_BYTE_LENGTH);
        rng.randomize(random_bytes.data(), random_bytes.size());
        
        return random_bytes;
    }
    
    std::string& base64_url_encode(std::string& str) {
        // Replace characters
        std::replace(str.begin(), str.end(), '+', '-');
        std::replace(str.begin(), str.end(), '/', '_');
        
        // Move all '=' characters to the end, then deallocate that block of '=' characters
        str.erase(std::remove(str.begin(), str.end(), '='), str.end());
        
        return str;
    }
}
