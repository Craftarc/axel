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

namespace axel {
    class Database : public IDatabase {
    public:
        /// @brief Parameterised constructor. All Database objects must be constructed with a specified table name.
        Database(std::string table_name);
        /// @brief Stores items in the database
        bool put(const std::unordered_map<Aws::String, Aws::String>& items) override;
        
        /// @brief Retrieves an item from a database using a given value for its partition key
        [[nodiscard]] Aws::Map<Aws::String, Aws::DynamoDB::Model::AttributeValue>
        get(const Aws::String& key_value) const override;
        
        /// @param key_value The key_value of the partition key of the item to delete
        /// @brief Removes the item with the specified partition key attribute from the database
        bool del(const Aws::String& key_value) const override;
    
    private:
        const Aws::DynamoDB::DynamoDBClient client_;
        const std::string table_name_;
        const Aws::String partition_key_;
        
        /// @brief Retrieve the name of the partition key of the table.
        Aws::String get_partition_key() const;
        
        /// @brief Constructs a DynamoDB client for use within this class.
        Aws::DynamoDB::DynamoDBClient connect(const std::string& database);
    };
} // axel

#endif //AXEL_AXEL_DATABASE_H