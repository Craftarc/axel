#include <iostream>
#include <string>
#include <aws/lambda-runtime/runtime.h>
#include <boost/json.hpp>
#include <fstream>

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
    
    /// Takes in a JSON file, then tries to read as many fields matching those in an invocation_request
    /// as possible, before creating a new invocation_request with those values filled in.
    /// @param filename Path to input JSON file.
    /// @return An invocation_request with member variables assigned
    /// @note Pass a std::ifstream to read a file with
    invocation_request make_invocation_request(const std::string& filename) {
        std::fstream file(filename);
        std::string input = parse::read_file_into_string(file);
        
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
    
    /// In converting to kebab-case, the following transformations are made:
    /// 1. Uppercase characters are changed to lowercase.
    /// 2. Whitespaces are changed to dashes.
    /// 3. Punctuation is removed.
    /// @param string String to convert.
    /// @return Kebab-cased string.
    std::string to_kebab(const std::string& string) {
        std::string kebab(string.length(), '\0'); // Result string to write to
        
        auto to_kebab = [](unsigned char c) -> unsigned char {
            if (std::isupper(c)) { // Upper to lower
                return std::tolower(c);
            } else if (std::isblank(c)) { // Whitespace to dash
                return '-';
            } else {
                return c;
            }
        };
        std::transform(string.begin(), string.end(), kebab.begin(), to_kebab);
        
        // Remove punctuation with remove-and-erase
        auto end{std::remove(kebab.begin(), kebab.end(), '\'')};
        kebab.erase(end, kebab.end());
        
        return kebab;
    }
} // parse