#ifndef AXEL_AXEL_DYNAMODBCLIENT_H
#define AXEL_AXEL_DYNAMODBCLIENT_H

#include <aws/dynamodb/DynamoDBClient.h>

#include "interface/IDynamoDBClient.h"

namespace axel {
    class DynamoDBClient : public IDynamoDBClient {
    public:
        /// @brief Parameterised constructor.
        DynamoDBClient(const std::string& table_name);
        
        /// @brief Wrapper over AWS's DynamoDBClient with the same method signature.
        Aws::DynamoDB::Model::PutItemOutcome
        PutItem(const Aws::DynamoDB::Model::PutItemRequest& request) const override;
        
        /// @brief Wrapper over AWS's DynamoDBClient with the same method signature.
        Aws::DynamoDB::Model::GetItemOutcome
        GetItem(const Aws::DynamoDB::Model::GetItemRequest& request) const override;
        
        /// @brief Wrapper over AWS's DynamoDBClient with the same method signature.
        Aws::DynamoDB::Model::DescribeTableOutcome
        DescribeTable(const Aws::DynamoDB::Model::DescribeTableRequest& request) const;
        
        /// @brief Wrapper over AWS's DynamoDBClient with the same method signature.
        Aws::DynamoDB::Model::DeleteItemOutcome
        DeleteItem(const Aws::DynamoDB::Model::DeleteItemRequest& request) const;
    private:
        /// @brief Sets the table the DynamoDBClient will interact with
        Aws::DynamoDB::DynamoDBClient connect(const std::string& database);
        
        Aws::DynamoDB::DynamoDBClient client_;
    };
}
#endif // AXEL_AXEL_DYNAMODBCLIENT_H