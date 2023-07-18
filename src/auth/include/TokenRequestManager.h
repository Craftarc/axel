#ifndef AXEL_WEBUTIL_TOKENREQUESTMANAGER_H
#define AXEL_WEBUTIL_TOKENREQUESTMANAGER_H

#include <boost/beast/http.hpp>

namespace webutil {
    namespace http = boost::beast::http;
    
    class TokenRequestManager {
        
        /// @brief Responsible for constructing and storing the POST request for the access token exchange.
        /// Contains all relevant fields as specified in the PoE Developer Docs.
        /// Relevant documentation: https://www.pathofexile.com/developer/docs/authorization
    public:
        /// @brief Constructs an access token exchange request.
        TokenRequestManager();
        
        /// @brief Gets the access token request.
        http::request<http::string_body> get_token_request() const;
        
        /// @brief Constructs an HTTP POST request for the exchange for the access token
        http::request<http::string_body> make_token_request(std::string auth_code, std::string code_verifier);
    
    private:
        
        /// @brief Stores the access tokne request
        http::request<http::string_body> token_request_;
    };
}

#endif //AXEL_WEBUTIL_TOKENREQUESTMANAGER_H
