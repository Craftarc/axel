#include "axel/Database.h"

#include <aws/dynamodb/DynamoDBClient.h>
#include <gmock/gmock.h>

#include "axel/MockDynamoDBClient.h"
#include "config/axel.h"

using namespace ::testing;
namespace Model = Aws::DynamoDB::Model;

namespace {

	// A KeySchemaElement with key type HASH (the partition key type) of name "session_id"
	Model::KeySchemaElement key_schema_element{
		Model::KeySchemaElement().WithAttributeName("session_id").WithKeyType(Model::KeyType::HASH)
	};
	std::vector<Model::KeySchemaElement> key_schema{ key_schema_element };

	// A TableDescription with a key schema consisting of the above key schema elements
	Model::TableDescription table_description{ Model::TableDescription().WithKeySchema(key_schema) };

	// A DescribeTableResult with the above table description
	Model::DescribeTableResult describe_table_result{ Model::DescribeTableResult().WithTable(table_description) };

	// A successful DescribeTableOutcome
	Aws::DynamoDB::Model::DescribeTableOutcome successful_describe_table_outcome{ describe_table_result };

	Model::PutItemResult put_item_result;
	// A successful PutItemOutcome
	Model::PutItemOutcome successful_put_item_outcome{ put_item_result };

	Model::GetItemResult get_item_result;
	// A successful GetItemOutcome
	Model::GetItemOutcome successful_get_item_outcome{ get_item_result };
	// An unsuccessful GetItemOutcome
	Model::GetItemOutcome unsuccessful_get_item_outcome{ Aws::DynamoDB::DynamoDBError() };
}  // namespace

namespace axel {
	class Database_test : public Test {
		protected:
			Database_test() :
				mock_auth_client{ new NiceMock<axel::MockDynamoDBClient> },
				setup_return(setup()),
				auth_database{ config::axel::database::auth,
				               config::axel::database::auth_attributes,
				               std::unique_ptr<MockDynamoDBClient>(mock_auth_client) } {}

			// Run this before anything else in the initialiser list, since initialisation needs DynamoDBClient (so it needs mocks too)
			int setup() {
				EXPECT_CALL(*mock_auth_client, DescribeTable(_))
				.Times(1)
				.WillOnce(Return(successful_describe_table_outcome));
				return 1;
			}

			axel::MockDynamoDBClient* mock_auth_client;
			int setup_return;
			Database auth_database;
	};

	TEST_F(Database_test, put__error_attributes_not_valid_for_given_table) {
		EXPECT_THROW(auth_database.put({ { "invalid_attribute_name", "" } }), std::runtime_error);
	}

	TEST_F(Database_test, put__error_if_partition_key_value_not_in_list_of_items_to_add_to_table) {
		// List of items to add does not contain the "session_id" partition key and value
		std::unordered_map<Aws::String, Aws::String> items{
			{ { "state_hash", "" }, { "code_verifier", "" }, { "time_to_live", "" } }
		};

		EXPECT_THROW(auth_database.put(items), std::runtime_error);
	}

	TEST_F(Database_test, put__valid_if_partition_key_value_in_list_of_items_to_add_to_table) {
		// List of items contains the "session_id" partition key-value pair
		std::unordered_map<Aws::String, Aws::String> items{
			{ { "session_id", "" }, { "state_hash", "" }, { "code_verifier", "" }, { "time_to_live", "" } }
		};

		EXPECT_CALL(*mock_auth_client, PutItem(_)).Times(1).WillOnce(Return(successful_put_item_outcome));

		EXPECT_NO_THROW(auth_database.put(items));
	}

	TEST_F(Database_test, get__error_if_partition_key_does_not_exist_in_table) {
		EXPECT_CALL(*mock_auth_client, GetItem(_)).Times(1).WillOnce(Return(unsuccessful_get_item_outcome));
		EXPECT_THROW(auth_database.get("non_existent_key"), std::runtime_error);
	}

	TEST_F(Database_test, get__valid_if_partition_key_exists_in_table) {
		EXPECT_CALL(*mock_auth_client, GetItem(_)).Times(1).WillOnce(Return(successful_get_item_outcome));
		EXPECT_NO_THROW(auth_database.get("valid_key"));
	}
}  // namespace axel
