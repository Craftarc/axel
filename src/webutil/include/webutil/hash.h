#ifndef AXEL_WEBUTIL_HASH_H
#define AXEL_WEBUTIL_HASH_H

#include <vector>
#include <string>

namespace webutil {
    /// @brief Generate the secret bytes for the code verifier.
    std::vector<uint8_t> generate_secret_bytes();
    
    /// @brief Modify a base64 encoded string into a base64url encoded string.
    std::string& base64_url_encode(std::string& str);
}

#endif // AXEL_WEBUTIL_HASH_H