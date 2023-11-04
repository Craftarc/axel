#include "util/type.h"

#include "spdlog/spdlog.h"
#include "src/axel/include/axel/Exception.h"

#include <cmath>

namespace util {
    /// @param integer The int64_t to cast to a double.
    /// @return The safely casted double.
    /// @note Throws an error if the cast is unsafe.
    double int64_t_to_double(int64_t integer) {
        long double upper_bound{ std::pow(2, 53) };
        if (integer > upper_bound) {
            spdlog::error(
            "Attempting narrowing conversion from int64_t to double");
            throw axel::Exception("Narrowing conversion");
        } else {
            return static_cast<double>(integer);
        }
    }

    int64_t long_to_int64_t(long long_value) {
        if (long_value <= std::numeric_limits<std::int64_t>::max() &&
            long_value >= std::numeric_limits<std::int64_t>::min()) {
            return static_cast<std::int64_t>(long_value);
        } else {
            throw axel::Exception("Narrowing conversion");
        }
    }

    std::string get_string(std::variant<std::string, int, double> variant) {
        const std::string* handle = std::get_if<std::string>(&variant);
        if (handle) {
            return *handle;
        } else {
            throw axel::Exception("Underlying type was not a string");
        }
    }

    int get_int(std::variant<std::string, int, double> variant) {
        const int* handle = std::get_if<int>(&variant);
        if (handle) {
            return *handle;
        } else {
            throw axel::Exception("Underlying type was not a int");
        }
    }

    double get_double(std::variant<std::string, int, double> variant) {
        const double* handle = std::get_if<double>(&variant);
        if (handle) {
            return *handle;
        } else {
            throw axel::Exception("Underlying type was not a double");
        }
    }

}  // namespace util
