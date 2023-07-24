#include <aws/lambda-runtime/runtime.h>
#include <iostream>
#include <boost/json.hpp>
#include "auth/OauthManager.h"

using namespace aws::lambda_runtime;

static invocation_response my_handler(invocation_request const& req) {
    auth::OauthManager oauth_manager;
    std::string json_string = req.payload;
    auto value = boost::json::parse(json_string, boost::json::error_code{});
    auto target = value.at("queryStringParameters").at("code");
    std::cerr << target.as_string() << std::endl;
    
    if (req.payload.length() > 42) {
        return invocation_response::failure("error message here"/*error_message*/,
                                            "error type here" /*error_type*/);
    }
    
    return invocation_response::success("json payload here" /*payload*/,
                                        "application/json" /*MIME type*/);
}

int main() {
    run_handler(my_handler);
    return 0;
}