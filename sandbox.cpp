/*
   Copyright Amazon.com, Inc. or its affiliates. All Rights Reserved.
   SPDX-License-Identifier: Apache-2.0
*/
/**
 * Before running this C++ code example, set up your development environment, including your credentials.
 *
 * For more information, see the following documentation topic:
 *
 * https://docs.aws.amazon.com/sdk-for-cpp/v1/developer-guide/getting-started.html
 *
 * For information on the structure of the code examples and how to build and run the examples, see
 * https://docs.aws.amazon.com/sdk-for-cpp/v1/developer-guide/getting-started-code-examples.html.
 *
 **/
#include <fstream>
#include <string>
#include <aws/core/Aws.h>
#include <aws/lambda-runtime/runtime.h>

#include "auth/OauthManager.h"
#include "axel/TestHandler.h"
#include "parse/util.h"

int main() {
    Aws::SDKOptions options;
    Aws::InitAPI(options);
    {
        axel::TestHandler test_handler;
        std::fstream start_file("test_input/axel/login-request.json");
        std::fstream auth_response_file("test_input/axel/auth-response.json");
        std::string start_input = parse::read_file_into_string(start_file);
        std::string auth_response_input = parse::read_file_into_string(auth_response_file);
        
        // Initiate start
        invocation_request start_request = parse::make_invocation_request(start_input);
        auto res = test_handler.run(start_request); // should print response
        std::cout << res.get_payload() << std::endl;
//
//        // Initiate auth_response
//        invocation_request auth_response_request = parse::make_invocation_request(auth_response_input);
//        test_handler.run(auth_response_request);
    }
    Aws::ShutdownAPI(options);
    return 0;
}