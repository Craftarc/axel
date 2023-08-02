#include <aws/lambda-runtime/runtime.h>
#include <iostream>
#include <boost/json.hpp>
#include "axel/Handler.h"
#include "auth/OauthManager.h"

using namespace aws::lambda_runtime;
namespace {
    /// @brief Takes a request from the AWS Lambda server runtime and delegates it to the appropriate method to be processed.
    invocation_response handler(const invocation_request& req) {
        std::string payload = req.payload;
        std::cout << payload << std::endl;
        return invocation_response::success("", "application/json");
    }
}
namespace axel {
    void Handler::run() {
        aws::lambda_runtime::run_handler(handler);
    }
}
