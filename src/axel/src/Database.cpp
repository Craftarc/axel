#include <spdlog/spdlog.h>

#include <aws/dynamodb/model/PutItemRequest.h>
#include <aws/dynamodb/model/GetItemRequest.h>
#include <aws/dynamodb/model/DescribeTableRequest.h>
#include <aws/dynamodb/model/DeleteItemRequest.h>

#include "axel/Database.h"

namespace axel {
    Database::Database(std::string table_name)
            : client_{connect(table_name)},
              table_name_{table_name},
              partition_key_(get_partition_key()) {
        spdlog::info("Interface to database created. Database: {}", table_name);
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
        
        if (outcome.IsSuccess()) {
            spdlog::info("Successfully added item into table '{}'.", table_name_);
        } else {
            spdlog::error("Failed to add item into table '{}'.", table_name_);
            throw std::runtime_error("Error adding item into table");
        }
        
        return outcome.IsSuccess();
    }
    
    /// @param key_value The value of the partition key of the item to fetch.
    /// @return A map of the retrieved item's attributes names and their values
    Aws::Map<Aws::String, Aws::DynamoDB::Model::AttributeValue> Database::get(const Aws::String& key_value) const {
        Aws::DynamoDB::Model::GetItemRequest get_item_request;
        get_item_request.SetTableName(table_name_);
        
        auto attribute_value{Aws::DynamoDB::Model::AttributeValue().SetS(key_value)}; // Construct proper type for .AddKey
        get_item_request.AddKey(get_partition_key(), attribute_value);
        
        auto outcome{client_.GetItem(get_item_request)};
        if (outcome.IsSuccess()) {
            spdlog::info("Successfully retrieved item from table '{}'.", table_name_);
            return outcome.GetResult().GetItem();
        } else {
            spdlog::error("Could not get item with given partition key '{}'", partition_key_);
            throw std::runtime_error("Could not get item from table");
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
                    Aws::String partition_key{element.GetAttributeName()};
                    spdlog::info("Found partition key of table '{0}': '{1}'", table_name_, partition_key);
                    
                    return partition_key;
                }
            }
        } else {
            spdlog::error("Could not get partition key of table '{0}'", table_name_);
            throw std::runtime_error("Could not get partition key of table");
        }
    }
    
    /// @return True if the deletion operation was successful, False otherwise.
    bool Database::del(const Aws::String& key_value) const {
        Aws::DynamoDB::Model::DeleteItemRequest delete_item_request;
        delete_item_request.SetTableName(table_name_);
        
        // Construct proper type for value of .AddKey (key, value)
        auto attribute_value{Aws::DynamoDB::Model::AttributeValue().SetS(key_value)};
        delete_item_request.AddKey(get_partition_key(), attribute_value);
        
        auto outcome{client_.DeleteItem(delete_item_request)};
        if (outcome.IsSuccess()) {
            spdlog::info("Deleted item in table '{}'", table_name_);
        } else {
            spdlog::error("Failed to delete item in table '{}'", table_name_);
            throw std::runtime_error("Failed to delete item in table");
        }
        
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