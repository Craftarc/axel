#ifndef AXEL_WEBUITL_HTTPSENDER_H
#define AXEL_WEBUITL_HTTPSENDER_H

#include "webutil/interfaces/IHttpSender.h"

namespace webutil {
    namespace http = boost::beast::http;
    
    class HttpSender : public IHttpSender {
    public:
        /// @brief Sends a HTTP request and returns the body of the HTTP response in a string.
        std::string send_http_request(const http::request<http::string_body>& request, int max_body_size) override;
        // TODO: Add an overload for a default max_body_size
    };
}
#endif // AXEL_WEBUITL_HTTPSENDER_H