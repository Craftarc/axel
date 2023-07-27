#include <iostream>
#include <string>
#include <aws/lambda-runtime/runtime.h>
#include <boost/json.hpp>

#include "parse/string.h"

namespace parse {
    
    /// @param file_stream File stream to read from.
    /// @return String containing all file data.
    std::string read_file_into_string(std::istream& file_stream) {
        std::string string;
        char character;
        while (file_stream.get(character)) {
            string += character;
        }
        return string;
    }
    
    /// Takes in a JSON string, then tries to read as many fields matching those in an invocation_request
    /// as possible, before creating a new invocation_request with those values filled in.
    /// @param input JSON string to convert
    /// @return An invocation_request with member variables assigned
    invocation_request make_invocation_request(const std::string& input) {
        invocation_request request{};
        boost::json::object input_json = boost::json::parse(input).as_object();
        // Check for presence of each of the fields
        if (input_json.contains("request_id")) {
            request.request_id = boost::json::serialize(input_json.at("request_id"));
        }
        if (input_json.contains("function_arn")) {
            request.function_arn = boost::json::serialize(input_json.at("function_arn"));
        }
        if (input_json.contains("payload")) {
            request.payload = boost::json::serialize(input_json.at("payload"));
        }
        
        return request;
    }
}