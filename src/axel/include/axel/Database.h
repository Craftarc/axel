#ifndef AXEL_AXEL_DATABASE_H
#define AXEL_AXEL_DATABASE_H

#include <vector>
#include <string>
#include <aws/core/client/ClientConfiguration.h>
#include <aws/core/client/AWSClient.h>
#include <aws/dynamodb/DynamoDBServiceClientModel.h>
#include <aws/dynamodb/DynamoDBClient.h>

#include "config/axel.h"

namespace axel {
    class Database {
    public:
        /// @brief Parameterised constructor. All Database objects must be constructed with a specified table name.
        Database(const std::string& table_name, const Aws::Client::ClientConfiguration& client_config);
        /// @brief Stores items in the database
        bool put(const std::unordered_map<Aws::String, Aws::String>& items);
        
        /// @brief Retrieves an item from a database using a given key_value for its partition key
        [[nodiscard]] Aws::Map<Aws::String, Aws::DynamoDB::Model::AttributeValue>
        get(const Aws::String& key_value) const;
        
        /// @param key_value The key_value of the partition key of the item to delete
        /// @brief Removes the item with the specified partition key attribute from the database
        bool del(const Aws::String& key_value) const;
        
        /// @brief Connects to a database.
        static std::unique_ptr<Database> connect(const std::string& database);
    
    private:
        const Aws::DynamoDB::DynamoDBClient client_;
        const std::string table_name_;
        const Aws::String partition_key_;
        
        /// @brief Retrieve the name of the partition key of the table.
        Aws::String get_partition_key() const;
    };
} // axel

#endif //AXEL_AXEL_DATABASE_H