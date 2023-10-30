#include "util/Database.h"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

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

    // Get underlying string from variant
    // Throws if the underlying value was not a string
    std::string get_string(std::variant<std::string, int> variant) {
        const std::string* handle = std::get_if<std::string>(&variant);
        if (handle) {
            return *handle;
        } else {
            throw axel::Exception("Underlying type was not a string");
        }
    }

    // Get underlying int from variant
    // Throws if the underlying value was not an int
    int get_int(std::variant<std::string, int> variant) {
        const int* handle = std::get_if<int>(&variant);
        if (handle) {
            return *handle;
        } else {
            throw axel::Exception("Underlying type was not a int");
        }
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

TEST_F(Database_tests, select_row__selects) {
    database.insert_row("app", apptable_row);
    std::vector select_attributes{ "session_id"s, "time_to_live"s };
    auto result = database.select_row("app", "session_id", select_attributes);
    EXPECT_EQ(2, result.size());  // 2 keys: session_id and time_to_live

    // Get the "session_id" value out
    auto session_id = get_string(result.at("session_id"));
    EXPECT_EQ("session_id", session_id);

    // Get the "time_to_live" value out
    auto time_to_live = get_int(result.at("time_to_live"));
    EXPECT_EQ(100, time_to_live);
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
    auto time_to_live = get_int(result.at("time_to_live"));
    EXPECT_EQ(100, time_to_live);

    database.update_row("app", "session_id", "time_to_live", 200);
    result = database.select_row("app", "session_id", select_attributes);
    time_to_live = get_int(result.at("time_to_live"));
    EXPECT_EQ(200, time_to_live);
}

TEST_F(Database_tests, update_row__updates_row_with_string_value) {
    database.insert_row("app", apptable_row);
    std::vector select_attributes{ "session_id"s, "access_token"s };
    auto result = database.select_row("app", "session_id", select_attributes);
    auto access_token = get_string(result.at("access_token"));
    EXPECT_EQ("access_token", access_token);

    database.update_row("app",
                        "session_id",
                        "access_token",
                        "new_access_token");
    result = database.select_row("app", "session_id", select_attributes);
    access_token = get_string(result.at("access_token"));
    EXPECT_EQ("new_access_token", access_token);
}

TEST_F(Database_tests, update_row__no_such_primary_key_value) {
    database.insert_row("app", apptable_row);
    std::vector select_attributes{ "session_id"s, "access_token"s };
    auto result = database.select_row("app", "session_id", select_attributes);
    auto access_token = get_string(result.at("access_token"));
    EXPECT_EQ("access_token", access_token);

    database.update_row("app",
                        "invalid_primary_key_value",
                        "access_token",
                        "new_access_token");
    result = database.select_row("app", "session_id", select_attributes);
    access_token = get_string(result.at("access_token"));
    EXPECT_EQ("access_token", access_token);
}
