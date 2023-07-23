#ifndef AXEL_WEBUITL_HTTPSENDER_H
#define AXEL_WEBUITL_HTTPSENDER_H

#include "webutil/IHttpSender.h"

namespace webutil {
    namespace http = boost::beast::http;
    
    class HttpSender : IHttpSender {
    public:
        std::string send_http_request(const http::request<http::string_body>& request) override;
    };
}
#endif // AXEL_WEBUITL_HTTPSENDER_H