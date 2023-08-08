#include <gmock/gmock.h>
#include <aws/dynamodb/DynamoDBClient.h>

#include "axel/MockDynamoDBClient.h"
#include "axel/Database.h"
#include "config/axel.h"

using namespace ::testing;
namespace Model = Aws::DynamoDB::Model;

namespace {
// A KeySchemaElement with key type HASH (the partition key type) of name "session_id"
    Model::KeySchemaElement key_schema_element{Model::KeySchemaElement().WithAttributeName("session_id")
                                                                        .WithKeyType(Model::KeyType::HASH)};
    std::vector<Model::KeySchemaElement> key_schema{key_schema_element};

// A TableDescription with a key schema consisting of the above key schema elements
    Model::TableDescription table_description{Model::TableDescription().WithKeySchema(key_schema)};

// A DescribeTableResult with the above table description
    Model::DescribeTableResult describe_table_result{Model::DescribeTableResult().WithTable(table_description)};
    axel::MockDynamoDBClient* mock_auth_client(new axel::MockDynamoDBClient());
    Aws::DynamoDB::Model::DescribeTableOutcome successful_outcome{describe_table_result};
}

namespace axel {
    class Database_test : public Test {
    protected:
        Database_test()
                : setup_return(setup()), auth_database{config::axel::database::auth,
                                                       config::axel::database::auth_attributes,
                                                       std::move(std::unique_ptr<MockDynamoDBClient>(
                                                               mock_auth_client))} {}
        
        // Run this before anything else in the initialiser list, since initialisation needs DynamoDBClient (so it needs mocks too)
        int setup() {
            EXPECT_CALL(*mock_auth_client, DescribeTable(_)).Times(1).WillOnce(Return(successful_outcome));
            return 1;
        }
        
        int setup_return;
        Database auth_database;
    };
    
    TEST_F(Database_test, put__error_attributes_not_valid_for_given_table) {
        EXPECT_THROW(auth_database.put({{"invalid_attribute_name", ""}}), std::runtime_error);
    }
}