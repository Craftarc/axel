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

#include <aws/core/Aws.h>
#include <aws/dynamodb/DynamoDBClient.h>
#include <aws/dynamodb/model/CreateTableRequest.h>
#include <iostream>
#include <aws/core/auth/AWSCredentials.h>
#include <aws/dynamodb/model/ListTablesRequest.h>

#include "config/axel.h"
#include "axel/Database.h"

int main() {
    
    Aws::SDKOptions options;
    
    Aws::InitAPI(options);
    {
        Aws::Client::ClientConfiguration client_configuration;
        client_configuration.region = "us-west-1";
        axel::Database db{config::axel::database::auth, client_configuration};
        auto map = db.get("abc");
    }
    Aws::ShutdownAPI(options);
    return 0;
}
