#include "util/HttpSender.h"

#include <boost/asio/ssl.hpp>
#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <iostream>
#include <spdlog/spdlog.h>

#include "util/http.h"

namespace {
	constexpr int HTTP_VERSION = 11;  // HTTP 1.1
}  // namespace

namespace util {
	namespace beast = boost::beast;
	namespace http = beast::http;
	namespace net = boost::asio;
	namespace ssl = net::ssl;
	using tcp = net::ip::tcp;

	/// @param request The HTTP request to be sent, with all request fields furnished.
	/// @param max_body_size Largest accepted body size of HTTP response, in MB. Default: 8MB
	/// @return The response body as a string
	std::string HttpSender::send_http_request(const http::request<http::string_body>& request, int max_body_size) {
		int max_body_size_bytes = max_body_size * 1024 * 1024;  // Argument is in MB, this converts to B
		std::string host = request["host"];

		// Reject hostnames with protocols
		std::string target = "://";
		if (host.find(target) != std::string::npos) {  // If a match is found at any index
			throw std::runtime_error("Host field should not contain protocol");
		}

		// IO and SSL context for stream
		net::io_context ioc;
		ssl::context ctx(ssl::context::tlsv12_client);
		ctx.set_verify_mode(ssl::verify_peer);
		ctx.set_verify_callback(ssl::host_name_verification(host));
		ctx.load_verify_file("/etc/ssl/certs/ca-certificates.crt");

		ssl::stream<tcp::socket> stream(ioc, ctx);  // Connection stream

		// Resolve hostname
		tcp::resolver resolver(ioc);
        spdlog::info("Resolving host: {}", host);
		const auto endpoints = resolver.resolve(host, "443"); // Use HTTPS

		// Set SNI Hostname (many hosts need this to handshake successfully)
		if (!SSL_set_tlsext_host_name(stream.native_handle(), host.c_str())) {
			boost::system::error_code ec{ static_cast<int>(ERR_get_error()), boost::asio::error::get_ssl_category() };
			throw boost::system::system_error{ ec };
		}

		// Handshake
		net::connect(stream.next_layer(), endpoints);

		stream.handshake(ssl::stream<tcp::socket>::client);

		http::write(stream, request);

		// Receive and print the HTTPS response
		beast::flat_buffer buffer;
		http::response_parser<http::dynamic_body> parser;
		parser.body_limit(max_body_size_bytes);
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

	/// @param request HTTP request to be sent.
	/// @return Body of the HTTP response.
	std::string HttpSender::send_http_request(const http::request<http::string_body>& request) {
		int DEFAULT_MAX_BODY_SIZE = 10;
		return send_http_request(request, DEFAULT_MAX_BODY_SIZE);
	}
}  // namespace util
