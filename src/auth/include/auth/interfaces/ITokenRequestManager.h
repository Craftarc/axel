#ifndef AXEL_AUTH_INTERFACES_ITOKENREQUESTMANAGER
#define AXEL_AUTH_INTERFACES_ITOKENREQUESTMANAGER

#include "webutil/IHttpSender.h"

namespace auth {
    class ITokenRequestManager {
    public:
        virtual ~ITokenRequestManager() = default;
        
        virtual std::pair<std::string, std::string> send_token_request(std::string auth_code, std::string code_verifier,
                                                                       std::unique_ptr<webutil::IHttpSender> http_sender) const = 0;
    };
}

#endif // AXEL_AUTH_INTERFACES_ITOKENREQUESTMANAGER