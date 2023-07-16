#ifndef AXEL_HTTP_GET_H
#define AXEL_HTTP_GET_H

#include <string>
#include <boost/beast/http.hpp>
#include <unordered_map>
#include <boost/beast/http/message.hpp>

namespace webutil {
    namespace http = boost::beast::http;
    
    /**
    * @brief Function to send a HTTP request
    *
    * @param request The HTTP request to be sent
    *
    * @return The body of the HTTP response from the server as a string
    */
    
    std::string
    send_http_request(const http::request<http::string_body>& request);
    
    http::request<http::string_body>
    make_http_request(const std::string& http_verb, const std::string& path,
                      const std::unordered_map<std::string, std::string>& fields,
                      const std::string& body = "");
}

#endif //AXEL_HTTP_GET_H
