#ifndef AXEL_PARSE_JSON_H
#define AXEL_PARSE_JSON_H
#include <iostream>
#include <spdlog/spdlog.h>
#include <boost/json.hpp>

#include "parse/JsonResult.h"

namespace json = boost::json;

namespace parse {
    
    /// @param value The JSON value to look for the key in.
    /// @param target The key, in JSON pointer format.
    /// @return The JsonResult of the operation.
    /// @see parse::JsonResult
    template<typename T>
    typename std::enable_if<parse::is_json_type<T>::value, JsonResult<T>>::type
    get(const json::value& value, const std::string& target) {
        boost::system::error_code ec;
        const json::value* target_value = value.find_pointer(target, ec);
        if (ec.value() != 0) { // Target key was not found
            return {T(), 1};
        } else { // Target key was found
            // If target_underlying != nullptr, the underlying representation exists
            const T* target_underlying{nullptr};
            // Return the result with the appropriate exit code
            auto exit = [&target_underlying]() -> JsonResult<T> {
                if (target_underlying == nullptr) {
                    return {T(), 1}; // Default exit
                } else {
                    return {*target_underlying, 0};
                }
            };
            
            // Verify the target value is of the correct type
            // boost::json::value::if_*() sets the pointer to nullptr if the underlying representation is not found
            if constexpr (std::is_same<T, json::array>::value) {
                target_underlying = target_value->if_array();
                return exit();
            } else if constexpr (std::is_same<T, json::object>::value) {
                target_underlying = target_value->if_object();
                return exit();
            } else if constexpr (std::is_same<T, json::string>::value) {
                target_underlying = target_value->if_string();
                return exit();
            } else if constexpr (std::is_same<T, bool>::value) {
                target_underlying = target_value->if_bool();
                return exit();
            } else if constexpr (std::is_same<T, double>::value) {
                target_underlying = target_value->if_double();
                return exit();
            } else if constexpr (std::is_same<T, int64_t>::value) {
                target_underlying = target_value->if_int64();
                return exit();
            } else if constexpr (std::is_same<T, uint64_t>::value) {
                target_underlying = target_value->if_uint64();
                return exit();
            } else {
                return exit();
            }
        };
    }
}
#endif // AXEL_PARSE_JSON_H
