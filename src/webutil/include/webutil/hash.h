#ifndef AXEL_WEBUTIL_HASH_H
#define AXEL_WEBUTIL_HASH_H

#include <vector>
#include <string>

namespace webutil {
    /**
    * @brief Generate the secret bytes for the code verifier.
    *
    * @return 32 random bytes in a container. Cryptographically secure.
    */
    std::vector<uint8_t> generate_secret_bytes();
    
    /**
    * @brief Modify a base64 encoded string into a base64url encoded string.
    *
    * The modification follows the Notes on Implementing Base64url Encoding without Padding
    * outlined in RFC 7636 Appendix A. '+' is replaced to '-', '/' is replaced with '_', and '=' is removed and
    * left to the decoder to imply.
    *
    * @param str base64 encoded string. The string typically contains '=' as terminating characters
    * if padding was used.
    *
    * @return base64url encoded string
    *
    * @note It is assumed that str will not contain any non-padding '=' characters because it is base64 encoded.
    *
    */
    std::string& base64_url_encode(std::string& str);
}

#endif // AXEL_WEBUTIL_HASH_H