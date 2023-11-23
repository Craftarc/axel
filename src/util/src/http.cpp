#include "util/http.h"

#include <string>
#include <unordered_map>

#include <boost/asio/ip/tcp.hpp>
#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>

namespace {
    constexpr int HTTP_VERSION = 11;  // HTTP 1.1
    const std::string HTTP_SERVICE = "https";
}  // namespace

namespace util {
    namespace beast = boost::beast;
    namespace http = beast::http;
    namespace net = boost::asio;
    namespace ssl = net::ssl;
    using tcp = net::ip::tcp;

    http::request<http::string_body>
    make_http_request(const std::string& http_verb,
                      const std::string& path,
                      const std::unordered_map<std::string, std::string>&
                      fields,
                      const std::string& body) {
        http::request<http::string_body>
        request(http::string_to_verb(http_verb), path, HTTP_VERSION);

        for (auto& field : fields) {
            request.set(http::string_to_field(field.first), field.second);
        }
        request.body() = body;

        return request;
    }
}  // namespace util
