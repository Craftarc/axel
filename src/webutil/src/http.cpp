#include "webutil/http.h"

#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/asio/ssl.hpp>
#include <boost/asio/connect.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/ssl/error.hpp>
#include <boost/asio/ssl/stream.hpp>
#include <openssl/ssl.h>
#include <string>
#include <unordered_map>

namespace {
    constexpr int HTTP_VERSION = 11; // HTTP 1.1
    const std::string HTTP_SERVICE = "https";
    
    // Maximum size, in bytes, of the HTTP body the parser is willing to accept
    // TODO: Move this constant somewhere nicer so it doesn't have to affect all HTTP gets.
    constexpr int MAX_HTTP_BODY_SIZE = 15 * 1024 * 1024; // 15MB
    
}

namespace webutil {
    namespace beast = boost::beast;
    namespace http = beast::http;
    namespace net = boost::asio;
    namespace ssl = net::ssl;
    using tcp = net::ip::tcp;
    
    /// @param request The HTTP request to be sent
    /// @return The body of the HTTP response from the server as a string
    std::string
    send_http_request(const http::request<http::string_body>& request) {
        std::string host = request["host"];
        
        // IO and SSL context for stream
        net::io_context ioc;
        ssl::context ctx(ssl::context::tlsv12_client);
        ctx.set_verify_mode(ssl::verify_peer);
        ctx.set_verify_callback(ssl::host_name_verification(host));
        ctx.set_default_verify_paths();
        
        ssl::stream<tcp::socket> stream(ioc, ctx); // Connection stream
        
        // Resolve hostname
        tcp::resolver resolver(ioc);
        const auto endpoints = resolver.resolve(host, HTTP_SERVICE);
        
        // Set SNI Hostname (many hosts need this to handshake successfully)
        if (!SSL_set_tlsext_host_name(stream.native_handle(), host.c_str())) {
            boost::system::error_code ec{static_cast<int>(ERR_get_error()), boost::asio::error::get_ssl_category()};
            throw boost::system::system_error{ec};
        }
        
        // Handshake
        net::connect(stream.next_layer(), endpoints);
        
        stream.handshake(ssl::stream<tcp::socket>::client);
        
        http::write(stream, request);
        
        // Receive and print the HTTPS response
        beast::flat_buffer buffer;
        http::response_parser<http::dynamic_body> parser;
        parser.body_limit(MAX_HTTP_BODY_SIZE);
        read(stream, buffer, parser);
        http::response<http::dynamic_body> response = parser.release();
        
        // Close the stream
        beast::error_code ec;
        stream.shutdown(ec);
        stream.lowest_layer().close(ec);
        
        // Convert response to string for parsing
        std::string body_string = beast::buffers_to_string(response.body().data());
        
        return body_string;
    }
    
    http::request<http::string_body>
    make_http_request(const std::string& http_verb, const std::string& path,
                      const std::unordered_map<std::string, std::string>& fields,
                      const std::string& body) {
        http::request<http::string_body> request(http::string_to_verb(http_verb), path, HTTP_VERSION);
        
        for (auto& field: fields) {
            request.set(http::string_to_field(field.first), field.second);
        }
        request.body() = body;
        
        return request;
    }
} // webutil
