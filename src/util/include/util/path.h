#ifndef AXEL_util_PATH_H
#define AXEL_util_PATH_H

#include <string>
#include <unordered_map>

#include <boost/algorithm/string.hpp>

#include <map>

namespace util {
    /// @brief Adds query parameters to a url string.
    std::string
    add_query_parameters(const std::string& url,
                         const std::unordered_map<std::string, std::string>&
                         parameters);

    /// @brief Extracts query parameters from a url string into a map.
    std::unordered_map<std::string, std::string>
    extract_query_params(const std::string& url);

    /// @brief Constructs a string of key-value pairs following the application/x-www-form-encoded format.
    std::string
    make_form_data(std::unordered_map<std::string, std::string> params);

    /// @brief Concatenates all strings in a list with the url-encoded whitespace, '%20'.
    std::string
    concatenate_with_space(const std::vector<std::string>& string_list);
}  // namespace util

#endif  // AXEL_util_PATH_H
