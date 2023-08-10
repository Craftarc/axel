#ifndef AXEL_AXEL_IDYNAMODBCLIENT_H
#define AXEL_AXEL_IDYNAMODBCLIENT_H

#include <aws/dynamodb/DynamoDBClient.h>

namespace axel {
    /// Wrapper class around DynamoDBClient
    class IDynamoDBClient {
    public:
        virtual ~IDynamoDBClient() = default;
        
        virtual Aws::DynamoDB::Model::PutItemOutcome
        PutItem(const Aws::DynamoDB::Model::PutItemRequest& request) const = 0;
        
        virtual Aws::DynamoDB::Model::GetItemOutcome
        GetItem(const Aws::DynamoDB::Model::GetItemRequest& request) const = 0;
        
        virtual Aws::DynamoDB::Model::DescribeTableOutcome
        DescribeTable(const Aws::DynamoDB::Model::DescribeTableRequest& request) const = 0;
        
        virtual Aws::DynamoDB::Model::DeleteItemOutcome
        DeleteItem(const Aws::DynamoDB::Model::DeleteItemRequest& request) const = 0;
    };
}

#endif // AXEL_AXEL_IDYNAMODBCLIENT_H