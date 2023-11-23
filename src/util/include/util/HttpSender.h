#ifndef AXEL_util_HTTPSENDER_H
#define AXEL_util_HTTPSENDER_H

#include "util/interfaces/IHttpSender.h"

namespace util {
    namespace http = boost::beast::http;

    class HttpSender : public IHttpSender {
        public:
            /// @brief Sends a HTTP request and returns the body of the HTTP response in a string.
            std::string
            send_http_request(const http::request<http::string_body>& request,
                              int max_body_size) override;

            /// @brief Sends a HTTP request and returns the body of the HTTP response in a string.
            std::string
            send_http_request(const http::request<http::string_body>& request);

            std::string send_http_only_request(const http::request<http::string_body>&
                                        request,
                                        int port);
    };
}  // namespace util
#endif  // AXEL_util_HTTPSENDER_H
