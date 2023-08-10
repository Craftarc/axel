#ifndef AXEL_AXEL_MOCKDYNAMODBCLIENT_H
#define AXEL_AXEL_MOCKDYNAMODBCLIENT_H

#include <gmock/gmock.h>
#include <aws/dynamodb/DynamoDBClient.h>

#include "axel/interface/IDynamoDBClient.h"

namespace Model = Aws::DynamoDB::Model;

namespace axel {
    class MockDynamoDBClient : public IDynamoDBClient {
    public:
        MOCK_METHOD(Model::PutItemOutcome, PutItem, (const Model::PutItemRequest& request), (const, override));
        MOCK_METHOD(Model::GetItemOutcome, GetItem, (const Model::GetItemRequest& request), (const, override));
        MOCK_METHOD(Model::DescribeTableOutcome,
                    DescribeTable,
                    (const Model::DescribeTableRequest& request),
                    (const, override));
        MOCK_METHOD(Model::DeleteItemOutcome, DeleteItem, (const Model::DeleteItemRequest& request), (const, override));
    };
}

#endif // AXEL_AXEL_MOCKDYNAMODBCLIENT_H