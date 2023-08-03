#include "axel/Database.h"

#include <aws/dynamodb/model/PutItemRequest.h>
#include <aws/dynamodb/model/GetItemRequest.h>
#include <aws/dynamodb/model/DescribeTableRequest.h>
#include <aws/dynamodb/model/DeleteItemRequest.h>

namespace axel {
    Database::Database(std::string table_name)
            : client_{connect(table_name)},
              table_name_{std::move(table_name)},
              partition_key_(get_partition_key()) {
    };
    
    /// Adds items to the DynamoDB specified by the table name provided in the constructor.
    /// @param items A map of items to add to the table. Each key-value pair in the map represents the target attribute,
    /// and the value of that attribute to add, respectively. All items in the map must be part of the same row.
    /// @return True if the operation was successful, False otherwise.
    bool Database::put(const std::unordered_map<Aws::String, Aws::String>& items) {
        Aws::DynamoDB::Model::PutItemRequest put_item_request;
        put_item_request.SetTableName(table_name_);
        
        for (const auto& pair: items) {
            auto value{Aws::DynamoDB::Model::AttributeValue().SetS(pair.second)}; // Construct proper type for .AddItem()
            put_item_request.AddItem(pair.first, value);
        }
        const Aws::DynamoDB::Model::PutItemOutcome outcome{client_.PutItem(put_item_request)};
        
        return outcome.IsSuccess();
    }
    
    /// @param key_value The key_value of the primary key of the item to fetch.
    /// @return A map of the retrieved item's attributes names and their values
    Aws::Map<Aws::String, Aws::DynamoDB::Model::AttributeValue> Database::get(const Aws::String& key_value) const {
        Aws::DynamoDB::Model::GetItemRequest get_item_request;
        get_item_request.SetTableName(table_name_);
        
        auto attribute_value{Aws::DynamoDB::Model::AttributeValue().SetS(key_value)}; // Construct proper type for .AddKey
        get_item_request.AddKey(get_partition_key(), attribute_value);
        
        auto outcome{client_.GetItem(get_item_request)};
        if (outcome.IsSuccess()) {
            return outcome.GetResult().GetItem();
        } else {
            throw std::runtime_error("Could not get item with given partition key key_value");
        }
    }
    
    /// @return The name of the partition key of the table
    Aws::String Database::get_partition_key() const {
        Aws::DynamoDB::Model::DescribeTableRequest describe_table_request;
        describe_table_request.SetTableName(table_name_);
        
        auto outcome{client_.DescribeTable(describe_table_request)};
        if (outcome.IsSuccess()) {
            // .GetKeySchema() returns the table's partition key and sort key in a vector
            for (const auto& element: outcome.GetResult().GetTable().GetKeySchema()) {
                if (element.GetKeyType() == Aws::DynamoDB::Model::KeyType::HASH) { // KeyType::HASH is the partition key
                    return element.GetAttributeName();
                }
            }
        } else {
            throw std::runtime_error("Could not get partition key of table");
        }
    }
    
    /// @return True if the deletion operation was successful, False otherwise.
    bool Database::del(const Aws::String& key_value) const {
        Aws::DynamoDB::Model::DeleteItemRequest delete_item_request;
        delete_item_request.SetTableName(table_name_);
        
        auto attribute_value{Aws::DynamoDB::Model::AttributeValue().SetS(key_value)}; // Construct proper type for .AddKey
        delete_item_request.AddKey(get_partition_key(), attribute_value);
        
        auto outcome{client_.DeleteItem(delete_item_request)};
        return outcome.IsSuccess();
    }
    
    /// @param database Name of the database table to connect to.
    /// @return A DynamoDB client.
    Aws::DynamoDB::DynamoDBClient Database::connect(const std::string& database) {
        Aws::Client::ClientConfiguration client_configuration;
        client_configuration.region = "us-west-1";
        
        return Aws::DynamoDB::DynamoDBClient{client_configuration};
    }
} // axel