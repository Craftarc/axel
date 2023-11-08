#include "util/Database.h"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "axel/Exception.h"
#include "util/type.h"

namespace {
    // Gets the number of rows in a table
    int get_row_count(sqlite3* db, const std::string& table_name) {
        std::string sql = "SELECT COUNT(*) FROM " + table_name;
        sqlite3_stmt* stmt;

        if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) !=
            SQLITE_OK) {
            throw axel::Exception("Error preparing query");
        }

        if (sqlite3_step(stmt) != SQLITE_ROW) {
            throw axel::Exception("Error executing query");
        }

        int row_count = sqlite3_column_int(stmt, 0);
        sqlite3_finalize(stmt);

        return row_count;
    }

}  // namespace

using namespace ::testing;
using namespace std::literals::string_literals;

class Database_tests : public ::testing::Test {
    protected:
        Database_tests() :
            init{ []() {
                std::remove("test.db");  // Remove the old test database
                return 0;
            }() },
            apptable_row{ "session_id", "access_token", 100 },
            oauthtable_row{ "session_id",
                            "state_hash",
                            "code_challenge",
                            "code_verifier",
                            100 },
            typical_oauthtable_row{
                "4engQl6X9bfjo_DfARAumxw2xUiHJX8mUl6j96M_9ig",
                "wUJ3zE1DnS55EHilHoLCYXbInpeYSS5vldd6vP32w0g",
                "EnfQ0UAFJnCbuMEPRjCwnkk-78Fu-O18ibZ3hecZmVM",
                "dBKL66tP1Ij1kXLePlNoT6VcO698YFuajYc5NF8mBQs",
                1699091737
            },
            database{ "test.db" } {  // New database file created
            // Get a handle to the new database
            if (sqlite3_open("test.db", &db) != SQLITE_OK) {
                throw axel::Exception("Unable to open test database");
            };
        }

        int init;
        util::Database database;
        sqlite3* db;
        database::AppTable apptable_row;
        database::OAuthTable oauthtable_row;
        database::OAuthTable typical_oauthtable_row;
};

TEST_F(Database_tests, insert_row__apptable_inserts_row) {
    EXPECT_EQ(0, get_row_count(db, "app"));
    database.insert_row("app", apptable_row);
    EXPECT_EQ(1, get_row_count(db, "app"));
}

TEST_F(Database_tests, insert_row__no_duplicate_primary_key) {
    EXPECT_EQ(0, get_row_count(db, "app"));
    database.insert_row("app", apptable_row);
    EXPECT_EQ(1, get_row_count(db, "app"));
    EXPECT_EQ(1, database.insert_row("app", apptable_row));
    EXPECT_EQ(1, get_row_count(db, "app"));
}

TEST_F(Database_tests, delete_row__deletes_row) {
    EXPECT_EQ(0, get_row_count(db, "app"));
    database.insert_row("app", apptable_row);
    EXPECT_EQ(1, get_row_count(db, "app"));
    database.delete_row("app", "session_id", "session_id");
    EXPECT_EQ(0, get_row_count(db, "app"));
}

TEST_F(Database_tests, delete_row__delete_from_empty_table) {
    EXPECT_EQ(0, get_row_count(db, "app"));
    // SQLite shouldn't complain about deleting from empty tables
    EXPECT_NO_THROW(database.delete_row("app", "session_id", "session_id"));
}

TEST_F(Database_tests, delete_row__delete_from_non_existent_table) {
    EXPECT_EQ(1, database.delete_row("qwerty", "qwerty", "qwerty"));
}

TEST_F(Database_tests, select_row__selects_in_apptable) {
    database.insert_row("app", apptable_row);
    std::vector<std::string> select_attributes{ "session_id", "time_to_live" };
    auto result = database.select_row("app", "session_id", select_attributes);
    EXPECT_EQ(2, result.size());  // 2 keys

    // Get the "session_id" value out
    auto session_id = util::get_string(result.at("session_id"));
    EXPECT_EQ("session_id", session_id);

    // Get the "time_to_live" value out
    auto time_to_live = util::get_int(result.at("time_to_live"));
    EXPECT_EQ(100, time_to_live);
}

TEST_F(Database_tests, select_row__selects_in_oauthtable) {
    database.insert_row("oauth", oauthtable_row);
    std::vector<std::string> select_attributes{ "session_id",
                                                "code_verifier",
                                                "code_challenge" };
    auto result = database.select_row("oauth", "session_id", select_attributes);
    EXPECT_EQ(3, result.size());  // 3 keys

    // Get the "session_id" value out
    auto session_id = util::get_string(result.at("session_id"));
    EXPECT_EQ("session_id", session_id);

    // Get the "code_verifier" value out
    auto code_verifier = util::get_string(result.at("code_verifier"));
    EXPECT_EQ("code_verifier", code_verifier);

    // Get the "code_challenge" value out
    auto code_challenge = util::get_string(result.at("code_challenge"));
    EXPECT_EQ("code_challenge", code_challenge);
}

TEST_F(Database_tests,
       select_row__selects_attributes_out_of_order_in_oauthtable) {
    database.insert_row("oauth", oauthtable_row);

    // code_verifier appears before code_challenge in the table
    std::vector<std::string> select_attributes{ "session_id",
                                                "code_challenge",
                                                "code_verifier" };
    auto result = database.select_row("oauth", "session_id", select_attributes);
    EXPECT_EQ(3, result.size());  // 3 keys

    // Get the "session_id" value out
    auto session_id = util::get_string(result.at("session_id"));
    EXPECT_EQ("session_id", session_id);

    // Get the "code_challenge" value out
    auto code_challenge = util::get_string(result.at("code_challenge"));
    EXPECT_EQ("code_challenge", code_challenge);

    // Get the "code_verifier" value out
    auto code_verifier = util::get_string(result.at("code_verifier"));
    EXPECT_EQ("code_verifier", code_verifier);
}

TEST_F(Database_tests, select_row__typical_row_oauthtable) {
    database.insert_row("oauth", typical_oauthtable_row);

    std::vector<std::string> select_attributes{ "session_id",
                                                "state_hash",
                                                "code_challenge",
                                                "code_verifier",
                                                "time_to_live" };
    auto result =
    database.select_row("oauth",
                        "4engQl6X9bfjo_DfARAumxw2xUiHJX8mUl6j96M_9ig",
                        select_attributes);

    EXPECT_EQ(5, result.size());  // 3 keys

    // Get the "session_id" value out
    auto session_id = util::get_string(result.at("session_id"));
    EXPECT_EQ("4engQl6X9bfjo_DfARAumxw2xUiHJX8mUl6j96M_9ig", session_id);

    // Get the "state_hash" value out
    auto state_hash = util::get_string(result.at("state_hash"));
    EXPECT_EQ("wUJ3zE1DnS55EHilHoLCYXbInpeYSS5vldd6vP32w0g", state_hash);

    // Get the "code_challenge" value out
    auto code_challenge = util::get_string(result.at("code_challenge"));
    EXPECT_EQ("EnfQ0UAFJnCbuMEPRjCwnkk-78Fu-O18ibZ3hecZmVM", code_challenge);

    // Get the "code_verifier" value out
    auto code_verifier = util::get_string(result.at("code_verifier"));
    EXPECT_EQ("dBKL66tP1Ij1kXLePlNoT6VcO698YFuajYc5NF8mBQs", code_verifier);

    // Get the "time_to_live" value out
    auto time_to_live = util::get_int(result.at("time_to_live"));
    EXPECT_EQ(1699091737, time_to_live);
}

TEST_F(Database_tests, select_row__no_such_row) {
    database.insert_row("app", apptable_row);
    std::vector select_attributes{ "session_id"s, "time_to_live"s };
    EXPECT_NO_THROW(database.select_row("app",
                                        "nonexistent-value",
                                        select_attributes));
}

TEST_F(Database_tests, update_row__updates_row_with_int_value) {
    database.insert_row("app", apptable_row);
    std::vector select_attributes{ "session_id"s, "time_to_live"s };
    auto result = database.select_row("app", "session_id", select_attributes);
    auto time_to_live = util::get_int(result.at("time_to_live"));
    EXPECT_EQ(100, time_to_live);

    database.update_row("app", "session_id", "time_to_live", 200);
    result = database.select_row("app", "session_id", select_attributes);
    time_to_live = util::get_int(result.at("time_to_live"));
    EXPECT_EQ(200, time_to_live);
}

TEST_F(Database_tests, update_row__updates_row_with_string_value) {
    database.insert_row("app", apptable_row);
    std::vector select_attributes{ "session_id"s, "access_token"s };
    auto result = database.select_row("app", "session_id", select_attributes);
    auto access_token = util::get_string(result.at("access_token"));
    EXPECT_EQ("access_token", access_token);

    database.update_row("app",
                        "session_id",
                        "access_token",
                        "new_access_token");
    result = database.select_row("app", "session_id", select_attributes);
    access_token = util::get_string(result.at("access_token"));
    EXPECT_EQ("new_access_token", access_token);
}

TEST_F(Database_tests, update_row__no_such_primary_key_value) {
    database.insert_row("app", apptable_row);
    std::vector select_attributes{ "session_id"s, "access_token"s };
    auto result = database.select_row("app", "session_id", select_attributes);
    auto access_token = util::get_string(result.at("access_token"));
    EXPECT_EQ("access_token", access_token);

    database.update_row("app",
                        "invalid_primary_key_value",
                        "access_token",
                        "new_access_token");
    result = database.select_row("app", "session_id", select_attributes);
    access_token = util::get_string(result.at("access_token"));
    EXPECT_EQ("access_token", access_token);
}
