#ifndef AXEL_PARSE_STRING_H
#define AXEL_PARSE_STRING_H

#include <aws/lambda-runtime/runtime.h>
#include <istream>

using namespace aws::lambda_runtime;
namespace parse {
    /// @brief Read an entire file into a string.
    std::string read_file_into_string(std::istream& file_stream);
    
    /// @brief Creates an invocation_request using information in a provided JSON string.
    invocation_request make_invocation_request(const std::string& input);
    
    /// @brief Creates a JSON string representing an invocation_response payload to pass back to the AWS Lambda service.
    std::string make_invocation_response_payload(int status_code,
                                                 const std::unordered_map<std::string, std::string>& headers,
                                                 const std::string& body);
} // parse

#endif // AXEL_PARSE_STRING_H
