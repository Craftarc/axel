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
        invocation_request request = parse::make_invocation_request("test_input/auth/auth-response.json");

        axel::TestHandler::run(request);
    }
    Aws::ShutdownAPI(options);
    return 0;
}