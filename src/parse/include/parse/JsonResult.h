#ifndef AXEL_PARSE_JSONRESULT_H
#define AXEL_PARSE_JSONRESULT_H

#include <boost/json.hpp>

namespace json = boost::json;

namespace parse {
    /// @brief Type traits for JsonResult
    template<typename T>
    struct is_json_type : std::disjunction<std::is_same<T, json::array>,
                                           std::is_same<T, json::object>,
                                           std::is_same<T, json::string>,
                                           std::is_same<T, bool>,
                                           std::is_same<T, double>,
                                           std::is_same<T, int64_t>,
                                           std::is_same<T, uint64_t>> {};

    template<typename T>
    class JsonResult {
        public:
            /// @brief Default constructor.
            /// An object that contains the result of JSON searching operations.
            /// @param result Result of the search, in STL types or primitives
            /// (instead of boost::json::value).
            /// @param ec Error code. The operation failed if ec != 0.

            JsonResult(T result, int ec) :
                result_(std::move(result)),
                success_{ ec == 0 } {
                static_assert(is_json_type<T>::value,
                              "Template argument must be a Boost::JSON type");
            }

            /// @brief Returns if the query for the JSON key was successful or not.
            bool is_success() {
                return success_;
            }

            /// @brief Gets the JSON result in the correct type
            /// (array, object, etc.) instead of value
            const T& get() {
                return result_;
            }

        private:
            const T result_;
            bool success_;
    };
}  // namespace parse

#endif  // AXEL_PARSE_JSONRESULT_H
