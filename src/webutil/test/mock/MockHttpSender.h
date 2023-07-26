#ifndef AXEL_WEBUTIL_MOCKHTTPSENDER_H
#define AXEL_WEBUTIL_MOCKHTTPSENDER_H

#include <gmock/gmock.h>
#include <boost/beast/http.hpp>

#include "webutil/interfaces/IHttpSender.h"

namespace webutil {
    namespace http = boost::beast::http;
    
    class MockHttpSender : public IHttpSender {
    public:
        MOCK_METHOD(std::string, send_http_request, (const http::request<http::string_body>& request), (override));
    };
}

#endif // AXEL_WEBUTIL_MOCKHTTPSENDER_H