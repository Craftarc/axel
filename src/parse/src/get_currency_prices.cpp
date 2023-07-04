#include "get_currency_prices.h"

#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/ssl.hpp>
#include <boost/asio/connect.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/ssl/error.hpp>
#include <boost/asio/ssl/stream.hpp>
#include <openssl/ssl.h>
#include <iostream>
#include <string>

namespace {
    namespace beast = boost::beast;
    namespace http = beast::http;
    namespace net = boost::asio;
    namespace ssl = net::ssl;
    using tcp = net::ip::tcp;

    /**
    * Function to run HTTP Get to the specified host, service and target
    * This function is used to run a HTTPS GET request using Boost library.
    *
    * @param host - hostname or IP address of the server
    * @param service - service name or port number
    * @param target - The path on the server
    *
    * @return The body of the HTTP response from the server as a string
    */
    std::string
    http_get(const std::string& host, const std::string& service, const std::string& target) {


        // IO and SSL context for stream
        net::io_context ioc;
        ssl::context ctx(ssl::context::tlsv12_client);
        ssl::stream<tcp::socket> stream(ioc, ctx); // Connection stream

        // Resolve hostname
        tcp::resolver resolver(ioc);
        const auto endpoints = resolver.resolve(host, service);

        // Set SNI Hostname (many hosts need this to handshake successfully)
        if (!SSL_set_tlsext_host_name(stream.native_handle(), host.c_str())) {
            boost::system::error_code ec{static_cast<int>(ERR_get_error()), boost::asio::error::get_ssl_category()};
            throw boost::system::system_error{ec};
        }

        // Handshake
        net::connect(stream.next_layer(), endpoints);

        stream.handshake(ssl::stream_base::client);

        // Construct HTTP request
        http::request<http::string_body> request(http::verb::get, target, 11);
        request.set(http::field::host, host);

        http::write(stream, request);

        // Receive and print the HTTPS response
        beast::flat_buffer buffer;
        http::response<http::dynamic_body> response;
        http::read(stream, buffer, response);

        // Gracefully close the stream
        beast::error_code ec;
        stream.shutdown(ec);
        stream.lowest_layer().close(ec);

        // Convert response to string for parsing
        std::string body_string = beast::buffers_to_string(response.body().data());

        return body_string;
    }
}

namespace parse {
    void get_currency_json() {
        std::cout << http_get(parse::poeninja_host, "https", parse::poeninja_currency_target) << std::endl;
    }
}
