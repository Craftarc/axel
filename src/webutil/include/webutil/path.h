#ifndef AXEL_WEBUTIL_PATH_H
#define AXEL_WEBUTIL_PATH_H

#include <string>
#include <map>
#include <unordered_map>

#include <boost/algorithm/string.hpp>

namespace pathutil {
    /// @brief Adds query parameters to a url string.
    std::string
    add_query_parameters(const std::string& url, const std::unordered_map<std::string, std::string>& parameters);
    
    /// @brief Extracts query parameters from a url string into a map.
    std::unordered_map<std::string, std::string> extract_query_params(const std::string& url);
    
    /// @brief Construct a string of key-value pairs following the application/x-www-form-encoded format.
    std::string make_form_data(std::unordered_map<std::string, std::string> params);
}

#endif // AXEL_WEBUTIL_PATH_H
