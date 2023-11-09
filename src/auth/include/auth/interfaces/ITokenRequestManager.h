#ifndef AXEL_AUTH_ITOKENREQUESTMANAGER
#define AXEL_AUTH_ITOKENREQUESTMANAGER

#include "util/interfaces/IHttpSender.h"

namespace auth {
    class ITokenRequestManager {
        public:
            virtual ~ITokenRequestManager() = default;

            virtual std::string
            send_token_request(std::string auth_code,
                               std::string code_verifier,
                               std::shared_ptr<util::IHttpSender> http_sender)
            const = 0;
    };
}  // namespace auth

#endif  // AXEL_AUTH_ITOKENREQUESTMANAGER
