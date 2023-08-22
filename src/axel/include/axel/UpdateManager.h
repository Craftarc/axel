#include <string>
#include <boost/json.hpp>
#include <boost/beast/http.hpp>

#ifndef AXEL_UPDATEMANAGER_H
#define AXEL_UPDATEMANAGER_H

#endif //AXEL_UPDATEMANAGER_H

namespace http = boost::beast::http;

namespace axel {
    class UpdateManager {
    public:
        /// @brief Default constructor.
        explicit UpdateManager(const std::string& access_token);
        
        /// @brief Call the PoE API for player inventory updates
        boost::json::value get_update() const;
        
        http::request<http::string_body> make_stash_request() const;
    
    private:
        const std::string& _access_token;
    };
} // axel