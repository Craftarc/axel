#include "util/path.h"

namespace util {

	/// Works with url strings regardless of if they have existing query parameters.
	///
	/// @param url The URL string
	/// @param parameters A map of keys and values to be added to the URL.
	/// @return A URL string with the specified parameters appended.

	std::string add_query_parameters(const std::string& url,
	                                 const std::unordered_map<std::string, std::string>& parameters) {
		std::string url_copy = url;
		if (parameters.empty())
			return url;

		size_t question_mark_position = url.find('?');
		if (question_mark_position == std::string::npos) {  // If the url doesn't have a query string yet
			url_copy += '?';
		} else {              // It already does have a query string
			url_copy += '&';  // Prepare for next set of parameters
		}

		// Add each key pair to the query
		for (const auto& parameter : parameters) {
			url_copy += parameter.first + '=' + parameter.second + '&';
		}

		// Remove trailing '&'
		url_copy.pop_back();

		return url_copy;
	};

	/// @param url The url string containing query parameters.
	/// @return Map of query parameters extracted from the URL string.
	std::unordered_map<std::string, std::string> extract_query_params(const std::string& url) {
		std::unordered_map<std::string, std::string> params;
		std::string query = url.substr(url.find('?') + 1);  // Consider only characters after '?'

		std::vector<std::string> query_components;
		boost::split(query_components, query, boost::is_any_of("&"));

		for (const auto& component : query_components) {
			std::vector<std::string> key_value_pair;
			boost::split(key_value_pair, component, boost::is_any_of("="));
			if (key_value_pair.size() == 2) {
				params[key_value_pair[0]] = key_value_pair[1];
			}
		}
		return params;
	}

	/// @param params Map of key-value pairs to be added to the string
	///
	/// @return The formatted string.
	std::string make_form_data(std::unordered_map<std::string, std::string> params) {
		std::string data;  // Stores the final string
		for (auto it = params.begin(); it != params.end(); ++it) {
			if (it == params.begin()) {  // Don't append '&' to the first key-value pair
				data += it->first + '=' + it->second;
			} else {
				data += '&' + it->first + '=' + it->second;
			}
		}
		return data;
	}

	/// @param string_list A list of strings to be concatenated
	/// @return The concatenated string.
	std::string concatenate_with_space(const std::vector<std::string>& string_list) {
		std::string result;
		for (auto it = std::begin(string_list); it != std::end(string_list); ++it) {
			// If iterator is at last element of vector don't add a trailing space
			if (it == std::prev(string_list.end())) {
				result += *it;
			} else {
				result += *it + "%20";  // ASCII encoding for whitespace
			}
		}

		return result;
	}
}  // namespace util