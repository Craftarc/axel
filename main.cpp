#include <spdlog/spdlog.h>
#include <sqlite3.h>

#include "auth/OauthManager.h"
#include "axel/ResourceManager.h"
#include "crow.h"
#include "crow/middlewares/cookie_parser.h"

int main() {
// Set logger output level
#ifdef NDEBUG  // Release
    spdlog::set_level(spdlog::level::info);
#else
    spdlog::set_level(spdlog::level::trace);
#endif

#ifdef AXEL_TEST
    std::string database{ "test.db" };
#else
    std::string database{ "app.db" };
#endif

    crow::App<crow::CookieParser> app;
    // Landing page
    CROW_ROUTE(app, "/")
    ([]() { return "root"; });

    // Auth start
    CROW_ROUTE(app, "/auth")
    ([&]() {
        crow::response response{ 404 };  // Default

        auth::OauthManager oauth_manager{ database };
        oauth_manager.start_auth(response);

        return response;
    });

    // Receive authorisation callback
    CROW_ROUTE(app, "/auth/callback")
    ([&](const crow::request& request) {
        crow::response response{ 404 };
        auth::OauthManager oauth_manager{ database };

        std::string url{ request.raw_url };  // Endpoint + query string
        spdlog::info("URL: {}", url);

        // Get session_id
        // See https://github.com/CrowCpp/Crow/blob/master/examples/middlewares/example_cookies.cpp
        auto& context = app.get_context<crow::CookieParser>(request);
        std::string session_id{ context.get_cookie("session_id") };
        spdlog::info("Session id: {}", session_id);
        oauth_manager.receive_auth(url, session_id, response);

        return response;
    });

    // Update
    CROW_ROUTE(app, "/update")
    ([&](const crow::request& request) {
        crow::response response{ 404 };
        auth::OauthManager oauth_manager{ database };

        std::string url{ request.raw_url };  // Endpoint + query string

        // Get axel_session_id
        auto& context = app.get_context<crow::CookieParser>(request);
        std::string axel_session_id{ context.get_cookie("axel_session_id") };
        spdlog::info("Axel session id: {}", axel_session_id);
        /* axel::ResourceManager resource_manager{ */
        /*     "d4336d2a46bb98d5e0dec8e7596dc62f386e8cd6", */
        /*     "", */
        /*     "" */
        /* }; */

        /* auto items = resource_manager.get_update(); */
        return 0;
    });

    app.port(80).multithreaded().run();
}
