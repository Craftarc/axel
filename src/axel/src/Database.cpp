#include <spdlog/spdlog.h>

#include <aws/dynamodb/model/PutItemRequest.h>
#include <aws/dynamodb/model/GetItemRequest.h>
#include <aws/dynamodb/model/DescribeTableRequest.h>
#include <aws/dynamodb/model/DeleteItemRequest.h>

#include "axel/Database.h"
#include "axel/DynamoDBClient.h"

namespace axel {
    
    /// Constructs an object that performs operations on the specified table name.
    /// @param table_name Name of the table this object interacts with.
    /// @param attributes Map of the attribute names, and the types of the attribute names, that this table stores.
    /// @param client The DynamoDBClient instance to use.
    /// @note This table only ever interacts with the table with the specified table name.
    Database::Database(const std::string& table_name,
                       const std::unordered_map<Aws::String, Aws::DynamoDB::Model::ValueType>& attributes,
                       std::unique_ptr<axel::IDynamoDBClient> client)
            : client_{std::move(client)},
              table_name_{table_name},
              partition_key_(get_partition_key()),
              attributes_{attributes} {};
    
    /// Constructs an object that performs operations on the specified table name.
    /// @param table_name Name of the table this object interacts with.
    /// @param attributes Map of the attribute names, and the types of the attribute names, that this table stores.
    /// @note Uses an instance of the AWS implementation of DynamoDBClient.
    Database::Database(const std::string& table_name,
                       const std::unordered_map<Aws::String, Aws::DynamoDB::Model::ValueType>& attributes) :
            Database(table_name, attributes, std::make_unique<axel::DynamoDBClient>(table_name)) {};
    
    /// Adds items to the DynamoDB specified by the table name provided in the constructor.
    /// @param items A map of items to add to the table. Each key-value pair in the map represents the target attribute,
    /// and the value of that attribute to add, respectively. All items in the map must be part of the same row.
    /// @return True if the operation was successful, False otherwise.
    bool Database::put(const std::unordered_map<Aws::String, Aws::String>& items) {
        Aws::DynamoDB::Model::PutItemRequest put_item_request;
        put_item_request.SetTableName(table_name_);
        spdlog::debug("Table name: {}", table_name_);
        
        for (const auto& pair: items) {
            add_item(pair, put_item_request);
        }
        
        const Aws::DynamoDB::Model::PutItemOutcome outcome{client_->PutItem(put_item_request)};
        
        if (outcome.IsSuccess()) {
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
        get_item_request.AddKey(partition_key_, attribute_value);
        
        auto outcome{client_->GetItem(get_item_request)};
        if (outcome.IsSuccess()) {
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
        
        auto outcome{client_->DescribeTable(describe_table_request)};
        if (outcome.IsSuccess()) {
            // .GetKeySchema() returns the table's partition key and sort key in a vector
            for (const auto& element: outcome.GetResult().GetTable().GetKeySchema()) {
                if (element.GetKeyType() == Aws::DynamoDB::Model::KeyType::HASH) { // KeyType::HASH is the partition key
                    return element.GetAttributeName();
                }
            }
            spdlog::error("Could not get partition key of table '{0}'", table_name_);
            throw std::runtime_error("Could not get partition key of table");
        } else {
            spdlog::error("Failed partition key query on '{}'", table_name_);
            throw std::runtime_error("Failed partition key query on table");
        }
    }
    
    /// @return True if the deletion operation was successful, False otherwise.
    bool Database::del(const Aws::String& key_value) const {
        Aws::DynamoDB::Model::DeleteItemRequest delete_item_request;
        delete_item_request.SetTableName(table_name_);
        
        // Construct proper type for value of .AddKey (key, value)
        auto attribute_value{Aws::DynamoDB::Model::AttributeValue().SetS(key_value)};
        delete_item_request.AddKey(partition_key_, attribute_value);
        
        auto outcome{client_->DeleteItem(delete_item_request)};
        if (outcome.IsSuccess()) {
        } else {
            spdlog::error("Failed to delete item in table '{}'", table_name_);
            throw std::runtime_error("Failed to delete item in table");
        }
        
        return outcome.IsSuccess();
    }
    
    /// Appends the attribute name-value pair to be part of a PutItemRequest. Modifies the PutItemRequest directly.
    /// @param item_pair The name-value pair to add. Must be compliant with the types defined by attributes_
    /// @param put_item_request The PutItemRequest to add to.
    Aws::DynamoDB::Model::PutItemRequest&
    Database::add_item(const std::pair<Aws::String, Aws::String>& item_pair,
                       Aws::DynamoDB::Model::PutItemRequest& put_item_request) const {
        
        // Check if attribute name is in pre-defined valid attributes
        auto position = attributes_.find(item_pair.first);
        if (position == attributes_.end()) { // If attribute name is not found
            throw std::runtime_error("Database: Given item attribute name does not exist in table");
        } else {  // If found, then construct the right DynamoDB AttributeValue type for the attribute's ValueType
            using ValueType = Aws::DynamoDB::Model::ValueType;
            ValueType value_type = position->second; // Get AttributeValue type
            
            // Construct the right AttributeValue
            Aws::DynamoDB::Model::AttributeValue value;
            switch (value_type) {
                case ValueType::STRING:
                    value.SetS(item_pair.second);
                    break;
                case ValueType::NUMBER:
                    value.SetN(item_pair.second);
                    break;
                default:
                    spdlog::error("Undefined type of attribute value");
                    throw std::runtime_error("Undefined type of attribute value");
            }
            
            put_item_request.AddItem(item_pair.first, value);
            spdlog::debug("Database::add_item: Added ({0}, {1}) to PutItemRequest", item_pair.first, item_pair.second);
            return put_item_request;
        }
    }
} // axel