#ifndef AXEL_WEBUTIL_HTTPSENDER_H
#define AXEL_WEBUTIL_HTTPSENDER_H

#include "webutil/interfaces/IHttpSender.h"

namespace webutil {
    namespace http = boost::beast::http;
    
    class HttpSender : public IHttpSender {
    public:
        /// @brief Sends a HTTP request and returns the body of the HTTP response in a string.
        std::string send_http_request(const http::request<http::string_body>& request, int max_body_size) override;
        
        /// @brief Sends a HTTP request and returns the body of the HTTP response in a string.
        std::string send_http_request(const http::request<http::string_body>& request);
    };
}
#endif // AXEL_WEBUTIL_HTTPSENDER_H