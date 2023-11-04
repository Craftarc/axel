#include <spdlog/spdlog.h>
#include <sqlite3.h>

#include "auth/OauthManager.h"
#include "axel/ResourceManager.h"
#include "crow.h"

int main() {
// Set logger output level
#ifdef NDEBUG  // Release
    spdlog::set_level(spdlog::level::info);
#else
    spdlog::set_level(spdlog::level::trace);
#endif

    crow::SimpleApp app;
    // Landing page
    CROW_ROUTE(app, "/")
    ([]() { return "root"; });

    // Auth start
    CROW_ROUTE(app, "/auth")
    ([]() {
        crow::response response{ 404 }; // Default
        auth::OauthManager oauth_manager{};
        oauth_manager.start_auth(response);

        return response;
    });

    // Receive authorisation callback
    CROW_ROUTE(app, "/auth/callback")
    ([](const crow::request& request) {
        crow::response response{ 404 };
        std::string url{ request.url };
        auth::OauthManager oauth_manager{};
        auto session_id = request.get_header_value("Cookie");
        spdlog::info("Query string: {}", url);
        spdlog::info("Cookies: {}", session_id);
        oauth_manager.receive_auth(url, session_id, response);

        return response;
    });

    // Update
    CROW_ROUTE(app, "/update")
    ([]() {
        axel::ResourceManager resource_manager{
            "d4336d2a46bb98d5e0dec8e7596dc62f386e8cd6",
            "",
            ""
        };

        auto items = resource_manager.get_update();
        return 0;
    });

    app.port(8080).multithreaded().run();
}
