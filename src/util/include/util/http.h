#ifndef AXEL_util_HTTP_H
#define AXEL_util_HTTP_H

#include <string>
#include <unordered_map>

#include <boost/beast/http.hpp>
#include <boost/beast/http/message.hpp>

namespace util {
	namespace http = boost::beast::http;

	/// @brief Makes a HTTP request.
	http::request<http::string_body> make_http_request(const std::string& http_verb,
	                                                   const std::string& path,
	                                                   const std::unordered_map<std::string, std::string>& fields,
	                                                   const std::string& body = "");
}  // namespace util

#endif  //AXEL_util_HTTP_H
