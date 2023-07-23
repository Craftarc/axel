#ifndef AXEL_IHTTPSENDER_H
#define AXEL_IHTTPSENDER_H

#include <string>
#include <boost/beast/http.hpp>

namespace webutil {
    namespace http = boost::beast::http;
    
    class IHttpSender {
    public:
        virtual ~IHttpSender() = default;
        virtual std::string send_http_request(const http::request<http::string_body>& request) = 0;
    };
}
#endif // AXEL_IHTTPSENDER_H