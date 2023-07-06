#include "webutil/pathutil.h"

namespace pathutil {
    /**
     * @brief Modifies given URL to include query parameters
     *
     * @param url Target URL reference, will be modified by adding query parameters.
     * @param parameters Map containing pairs representing query parameters and their values.
     * @returns Updated URL with added query parameters. Returns original URL if input parameter map is empty.
     * @note Function modifies input URL string directly.
     */
    std::string
    add_query_parameters(std::string& url, const std::map<std::string, std::string>& parameters) {
        if (parameters.empty()) return url;

        size_t question_mark_position = url.find('?');
        if (question_mark_position == std::string::npos) { // If the url doesn't have a query string yet
            url += '?';
        } else { // It already does have a query string
            url += '&'; // Prepare for next set of parameters
        }

        // Add each key pair to the query
        for (const auto& parameter: parameters) {
            url += parameter.first + '=' + parameter.second + '&';
        }

        // Remove trailing '&'
        url.pop_back();

        return url;
    };
}