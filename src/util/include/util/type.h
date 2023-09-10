#ifndef AXEL_UTIL_TYPE_H
#define AXEL_UTIL_TYPE_H
#include <cstdint>

namespace util {
	/// @brief Safely convert an int64_t to a double
	double int64_t_to_double(int64_t integer);
}  // namespace util
#endif  // AXEL_UTIL_TYPE_H