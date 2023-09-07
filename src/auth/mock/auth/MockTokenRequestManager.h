#ifndef AXEL_AUTH_MOCKTOKENREQUESTMANAGER_H
#define AXEL_AUTH_MOCKTOKENREQUESTMANAGER_H

#include <gmock/gmock.h>

#include "auth/interfaces/ITokenRequestManager.h"
#include "util/interfaces/IHttpSender.h"

namespace auth {
	class MockTokenRequestManager : public ITokenRequestManager {
		public:
			MOCK_METHOD(std::string,
			            send_token_request,
			            (std::string auth_code,
			             std::string code_verifier,
			             std::unique_ptr<util::IHttpSender> http_sender),
			            (const, override));
	};
}  // namespace auth

#endif  // AXEL_AUTH_MOCKTOKENREQUESTMANAGER_H