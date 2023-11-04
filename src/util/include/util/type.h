#ifndef AXEL_UTIL_TYPE_H
#define AXEL_UTIL_TYPE_H
#include <cstdint>
#include <string>
#include <variant>

namespace util {
    /// @brief Safely convert an int64_t to a double
    double int64_t_to_double(int64_t integer);
    int64_t long_to_int64_t(long long_value);

    /// @brief Get the underlying string in a variant.
    /// @param variant Variant argument.
    /// @return The underlying string if present, an empty string otherwise.
    std::string get_string(std::variant<std::string, int, double> variant);

    /// @brief Get the underlying int in a variant.
    /// @param variant Variant argument.
    /// @return The underlying int if present, 0 otherwise.
    int get_int(std::variant<std::string, int, double> variant);

    /// @brief Get the underlying double in a variant.
    /// @param variant Variant argument.
    /// @return The underlying double if present, 0 otherwise.
    double get_double(std::variant<std::string, int, double> variant);
}  // namespace util
#endif  // AXEL_UTIL_TYPE_H
