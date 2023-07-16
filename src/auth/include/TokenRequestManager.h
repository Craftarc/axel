#ifndef AXEL_WEBUTIL_TOKENREQUESTMANAGER_H
#define AXEL_WEBUTIL_TOKENREQUESTMANAGER_H

#include <boost/beast/http.hpp>

namespace webutil {
    namespace http = boost::beast::http;
    
    class TokenRequestManager {
        /**
        * @internal
        * @brief Responsible for constructing and storing the POST request for the access token exchange.
        * Contains all relevant fields as specified in the PoE Developer Docs.
        * Relevant documentation: https://www.pathofexile.com/developer/docs/authorization
        */
    public:
        /**
         * @brief Constructor. Constructs an access token exchange request and stores it.
         * @param The authorization code to construct the request from.
         */
        TokenRequestManager();
        
        /**
         * @brief Gets the access token request.
         * @return The access token HTTP request.
         */
        http::request<http::string_body> get_token_request() const;
        
        /**
        * @brief Constructs an HTTP POST request for the exchange for the access token
        * and stores it in make_token_request_
        * @param auth_code The authorization code to be used in the request.
        * @return The fully constructed HTTP POST request.
        */
        http::request<http::string_body> make_token_request(std::string auth_code, std::string code_verifier);
    
    private:
        
        /**
         * @internal
         * @brief Stores the access token request
         */
        http::request<http::string_body> token_request_;
    };
}

#endif //AXEL_WEBUTIL_TOKENREQUESTMANAGER_H
