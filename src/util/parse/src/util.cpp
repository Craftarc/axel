#include <iostream>
#include <string>
#include <aws/lambda-runtime/runtime.h>
#include <boost/json.hpp>

#include "parse/util.h"

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
    
    /// @param status_code HTTP status code of response.
    /// @param headers Map of headers to include in response. 1-dimensional map only.
    /// @param body Payload body. Must be of "application/json" type.
    std::string make_invocation_response_payload(int status_code,
                                                 const std::unordered_map<std::string, std::string>& headers,
                                                 const std::string& body) {
        // Construct JSON object from headers map
        boost::json::object header_object;
        for (const auto& pair: headers) {
            header_object.emplace(pair.first, pair.second);
        }
        
        boost::json::value json{{"statusCode", status_code}, {"headers", std::move(header_object)}, {"body", body}};
        
        return boost::json::serialize(json); // Convert to string
    }
} // parse