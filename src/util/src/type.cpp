#include "util/type.h"

#include "spdlog/spdlog.h"

#include <cmath>

namespace util {
	/// @param integer The int64_t to cast to a double.
	/// @return The safely casted double.
	/// @note Throws an error if the cast is unsafe.
	double int64_t_to_double(int64_t integer) {
		long double upper_bound{ std::pow(2, 53) };
		if (integer > upper_bound) {
			spdlog::error("Attempting narrowing conversion from int64_t to double");
			throw std::runtime_error("Narrowing conversion");
		} else {
			return static_cast<double>(integer);
		}
	}
}  // namespace util