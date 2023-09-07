#include "util/hash.h"

#include <botan/auto_rng.h>

#include <algorithm>

namespace util {
	/**
     *
     * @return 32 random bytes in a container
     */
	std::vector<uint8_t> generate_secret_bytes() {
		const int VERIFIER_BYTE_LENGTH = 32;

		Botan::AutoSeeded_RNG rng;

		// Generate 32 random bytes
		std::vector<uint8_t> random_bytes(VERIFIER_BYTE_LENGTH);
		rng.randomize(random_bytes.data(), random_bytes.size());

		return random_bytes;
	}

	/**
    * The modification follows the Notes on Implementing Base64url Encoding without Padding
    * outlined in RFC 7636 Appendix A. '+' is replaced to '-', '/' is replaced with '_', and '=' is removed and
    * left to the decoder to imply.
    *
    * @param str base64 encoded string
    * @return base64url encoded string
    *
    * @note It is assumed that str will not contain any non-padding '=' characters because it is base64 encoded.
    */
	std::string& base64_url_encode(std::string& str) {
		// Replace characters
		std::replace(str.begin(), str.end(), '+', '-');
		std::replace(str.begin(), str.end(), '/', '_');

		// Move all '=' characters to the end, then deallocate that block of '=' characters
		str.erase(std::remove(str.begin(), str.end(), '='), str.end());

		return str;
	}
}  // namespace util
