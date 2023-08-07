#ifndef AXEL_AXEL_DATABASE_H
#define AXEL_AXEL_DATABASE_H

#include <vector>
#include <string>
#include <aws/core/client/ClientConfiguration.h>
#include <aws/core/client/AWSClient.h>
#include <aws/dynamodb/DynamoDBServiceClientModel.h>
#include <aws/dynamodb/DynamoDBClient.h>

#include "config/axel.h"
#include "axel/interface/IDatabase.h"
#include "axel/interface/IDynamoDBClient.h"

namespace axel {
    class Database : public IDatabase {
        friend class Database_test;
    
    public:
        /// @brief Parameterised constructor. Uses the AWS implementation of DynamoDBClient.
        Database(const std::string& table_name,
                 const std::unordered_map<Aws::String, Aws::DynamoDB::Model::ValueType>& attributes);
        
        /// @brief Stores items in the database
        bool put(const std::unordered_map<Aws::String, Aws::String>& items) override;
        
        /// @brief Retrieves an item from a database using a given value for its partition key
        [[nodiscard]] Aws::Map<Aws::String, Aws::DynamoDB::Model::AttributeValue>
        get(const Aws::String& key_value) const override;
        
        /// @param key_value The key_value of the partition key of the item to delete
        /// @brief Removes the item with the specified partition key attribute from the database
        bool del(const Aws::String& key_value) const override;
    
    protected:
        /// @brief Parameterised constructor. All Database objects must be constructed with a specified table name, and a
        /// set of valid attributes for that table.
        Database(const std::string& table_name,
                 const std::unordered_map<Aws::String, Aws::DynamoDB::Model::ValueType>& attributes,
                 std::unique_ptr<axel::IDynamoDBClient> client);
    
    private:
        std::unique_ptr<axel::IDynamoDBClient> client_;
        const std::string& table_name_;
        const Aws::String partition_key_;
        
        /// Table of |attribute name|attribute value| that defines the attributes of this table.
        const std::unordered_map<Aws::String, Aws::DynamoDB::Model::ValueType>& attributes_;
        
        /// @brief Retrieve the name of the partition key of the table.
        Aws::String get_partition_key() const;
        
        /// @brief Adds an attribute name-value pair to a PutItemRequest
        Aws::DynamoDB::Model::PutItemRequest& add_item(const std::pair<Aws::String, Aws::String>& item_pair,
                                                       Aws::DynamoDB::Model::PutItemRequest& put_item_request) const;
    };
} // axel

#endif //AXEL_AXEL_DATABASE_H