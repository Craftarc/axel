#include "get_currency_prices.h"

#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/ssl.hpp>
#include <boost/beast/version.hpp>
#include <boost/asio/connect.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/ssl/error.hpp>
#include <boost/asio/ssl/stream.hpp>
#include <cstdlib>
#include <iostream>
#include <string>


namespace {
    void http_get(const std::string &host) {
        namespace beast = boost::beast;
        namespace http = beast::http;
        namespace net = boost::asio;
        namespace ssl = net::ssl;
        using tcp = net::ip::tcp;

        // IO and SSL context for stream
        net::io_context ioc;
        ssl::context ctx(ssl::context::tlsv12_client);


        tcp::resolver resolver(ioc);
        ssl::stream<tcp::socket> socket(ioc, ctx); // Connection stream

        // Resolve hostname
        resolver.resolve(host, "https");

        // Handshake
        net::connect(socket.lowest_layer(), endpoints);
        socket.handshake(ssl::stream_base::client);

        // Construct HTTP request
        http::request<http::string_body> request(http::verb::get, "/", 11);
        request.set(http::field::host, host);
        request.set(http::field::user_agent, "Boost Beast");

        http::write(socket, request);

        // Receive and print the HTTPS response
        beast::flat_buffer buffer;
        http::response<http::dynamic_body> response;
        http::read(socket, buffer, response);
        std::cout << response << std::endl;

        // Gracefully close the socket
        beast::error_code ec;
        socket.shutdown(ec);
        socket.lowest_layer().close(ec);

        return 0;
    }

}

namespace parse {
    void get_currency_json() {
        http_get(parse::poeninja_currency_host);
    }
}
