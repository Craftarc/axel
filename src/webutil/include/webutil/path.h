#ifndef AXEL_WEBUTIL_PATH_H
#define AXEL_WEBUTIL_PATH_H

#include <string>
#include <map>
#include <unordered_map>

#include <boost/algorithm/string.hpp>

namespace pathutil {
    /**
     * @brief Adds query parameters to a url string.
     *
     * Works with url strings regardless of if they have existing query parameters.
     *
     * @param url The URL string
     * @param parameters A map of keys and values to be added to the URL.
     * @return A URL string with the specified parameters appended.
     */
    std::string
    add_query_parameters(const std::string& url, const std::unordered_map<std::string, std::string>& parameters);
    
    /**
     * @brief Extracts query parameters from a url string into a map.
     * @param url The url string containing query parameters.
     * @return Map of query parameters extracted from the URL string.
     */
    
    std::unordered_map<std::string, std::string> extract_query_params(const std::string& url);
    
    /**
     * @brief Construct a string of key-value pairs following the application/x-www-form-encoded format.
     * @param params Map of key-value pairs to be added to the string
     *
     * @return The formatted string.
     */
    std::string make_form_data(std::unordered_map<std::string, std::string> params);
}

#endif // AXEL_WEBUTIL_PATH_H
