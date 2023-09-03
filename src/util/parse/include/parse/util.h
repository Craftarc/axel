#ifndef AXEL_PARSE_UTIL_H
#define AXEL_PARSE_UTIL_H

#include <aws/lambda-runtime/runtime.h>
#include <istream>
#include <cctype>

using namespace aws::lambda_runtime;
namespace parse {
    /// @brief Read an entire file into a string.
    std::string read_file_into_string(std::istream& file_stream);
    
    /// @brief Creates an invocation_request using information in a provided JSON file.
    invocation_request make_invocation_request(const std::string& filename);
    
    /// @brief Creates a JSON string representing an invocation_response payload to pass back to the AWS Lambda service.
    std::string make_invocation_response_payload(int status_code,
                                                 const std::unordered_map<std::string, std::string>& headers,
                                                 const std::string& body);
    
    /// @brief Converts a string into kebab case
    std::string to_kebab(const std::string& string);
} // parse

#endif // AXEL_PARSE_UTIL_H
