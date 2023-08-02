#ifndef AXEL_AUTH_TOKENREQUESTMANAGER_H
#define AXEL_AUTH_TOKENREQUESTMANAGER_H

#include <boost/beast/http.hpp>
#include "webutil/interfaces/IHttpSender.h"
#include "auth/interfaces/ITokenRequestManager.h"

namespace auth {
    namespace http = boost::beast::http;
    
    class TokenRequestManager : public ITokenRequestManager {
        
        
        /// @brief Responsible for constructing and storing the POST request for the access token exchange.
        /// Contains all relevant fields as specified in the PoE Developer Docs.
        /// Relevant documentation: https://www.pathofexile.com/developer/docs/authorization
    public:
        /// @brief Constructs an access token exchange request.
        TokenRequestManager();
        
        /// @brief Sends the token exchange request furnished with the given arguments
        std::string
        send_token_request(std::string auth_code,
                           std::string code_verifier,
                           std::unique_ptr<webutil::IHttpSender> http_sender) const override;
    };
}

#endif //AXEL_AUTH_TOKENREQUESTMANAGER_H
