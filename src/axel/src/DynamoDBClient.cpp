#include "axel/DynamoDBClient.h"

namespace axel {
    /// @param table_name The name of the table the client should interact with.
    DynamoDBClient::DynamoDBClient(const std::string& table_name) : client_{connect(table_name)} {}
    
    /// @param database Name of the database table to connect to.
    /// @return A DynamoDB client.
    Aws::DynamoDB::DynamoDBClient DynamoDBClient::connect(const std::string& database) {
        Aws::Client::ClientConfiguration client_configuration;
        client_configuration.region = "us-west-1";
        
        return Aws::DynamoDB::DynamoDBClient{client_configuration};
    }
    
    Aws::DynamoDB::Model::PutItemOutcome
    DynamoDBClient::PutItem(const Aws::DynamoDB::Model::PutItemRequest& request) const {
        return client_.PutItem(request);
    }
    
    Aws::DynamoDB::Model::GetItemOutcome
    DynamoDBClient::GetItem(const Aws::DynamoDB::Model::GetItemRequest& request) const {
        return client_.GetItem(request);
    }
    
    Aws::DynamoDB::Model::DescribeTableOutcome
    DynamoDBClient::DescribeTable(const Aws::DynamoDB::Model::DescribeTableRequest& request) const {
        return client_.DescribeTable(request);
    }
    
    Aws::DynamoDB::Model::DeleteItemOutcome
    DynamoDBClient::DeleteItem(const Aws::DynamoDB::Model::DeleteItemRequest& request) const {
        return client_.DeleteItem(request);
    }
}
