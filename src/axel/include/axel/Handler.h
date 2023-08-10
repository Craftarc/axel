#ifndef AXEL_AXEL_HANDLER_H
#define AXEL_AXEL_HANDLER_H

#include <aws/lambda-runtime/runtime.h>

#include "auth/OauthManager.h"

using namespace aws::lambda_runtime;
namespace axel {
    class Handler {
    public:
        /// @brief Parameterised constructor. Sets up the Handler with the correct configurations, depending on if
        /// the code is meant to be run in the production environment or local testing.
        Handler(const std::string& environment);
        
        /// @brief Runs the handler implementation that communicates with the AWS Runtime Interface
        void run();
        
        /// @brief The handler invoked by the AWS Lambda runtime.
        invocation_response handler(const invocation_request& req);
    
    private:
        auth::OauthManager _oauth_manager;
        auth::OauthManager init(const std::string& environment);
    };
}

#endif // AXEL_AXEL_HANDLER_H