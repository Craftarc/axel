#ifndef AXEL_util_MOCKHTTPSENDER_H
#define AXEL_util_MOCKHTTPSENDER_H

#include <boost/beast/http.hpp>
#include <gmock/gmock.h>

#include "util/interfaces/IHttpSender.h"

namespace util {
	namespace http = boost::beast::http;

	class MockHttpSender : public IHttpSender {
		public:
			MOCK_METHOD(std::string,
			            send_http_request,
			            (const http::request<http::string_body>& request, int max_body_size),
			            (override));

			MOCK_METHOD(std::string, send_http_request, (const http::request<http::string_body>& request), (override));
	};
}  // namespace util

#endif  // AXEL_util_MOCKHTTPSENDER_H