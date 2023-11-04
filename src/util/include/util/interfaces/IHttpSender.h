#ifndef AXEL_UTIL_IHTTPSENDER_H
#define AXEL_UTIL_IHTTPSENDER_H

#include <string>

#include <boost/beast/http.hpp>

namespace util {
	namespace http = boost::beast::http;

	class IHttpSender {
		public:
			virtual ~IHttpSender() = default;
			virtual std::string send_http_request(const http::request<http::string_body>& request,
			                                      int max_body_size) = 0;
			virtual std::string send_http_request(const http::request<http::string_body>& request) = 0;
	};
}  // namespace util
#endif  // AXEL_UTIL_IHTTPSENDER_H
