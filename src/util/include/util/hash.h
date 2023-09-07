#ifndef AXEL_util_HASH_H
#define AXEL_util_HASH_H

#include <string>
#include <vector>

namespace util {
	/// @brief Generates the secret bytes for the code verifier.
	std::vector<uint8_t> generate_secret_bytes();

	/// @brief Modifies a base64 encoded string into a base64url encoded string.
	std::string& base64_url_encode(std::string& str);
}  // namespace util

#endif  // AXEL_util_HASH_H