#include "util/Database.h"

#include <variant>

#include <fmt/core.h>
#include <spdlog/spdlog.h>

#include "axel/Exception.h"
#include "config/database.h"

namespace {
    // Prints whatever comes out of the SQL query
    int callback(void*, int argc, char** argv, char** azColName) {
        for (int i = 0; i < argc; i++) {
            std::cout << azColName[i] << " = " << (argv[i] ? argv[i] : "NULL")
                      << std::endl;
        }
        return 0;
    }

}  // namespace

using TableInfo =
std::pair<std::string, std::unordered_map<std::string, std::string>>;

namespace util {
    Database::Database(const char* name) {
        if (sqlite3_open(name, &handle_) != SQLITE_OK) {
            throw axel::Exception("Could not open database file");
        }

        // Schema for oauth table
        // The oauth table stores details needed for the initial oauth step
        // Once the access token is retrieved successfully, it is stored in
        // the 'app' table
        execute("CREATE TABLE IF NOT EXISTS oauth"
                "(session_id TEXT PRIMARY KEY,"
                "state_hash TEXT UNIQUE NOT NULL,"
                "code_challenge TEXT UNIQUE NOT NULL,"
                "code_verifier TEXT UNIQUE NOT NULL,"
                "time_to_live INTEGER NOT NULL)");

        // Schema for app table
        execute("CREATE TABLE IF NOT EXISTS app"
                "(session_id TEXT PRIMARY KEY,"
                "access_token TEXT UNIQUE DEFAULT NULL,"
                "time_to_live INTEGER NOT NULL,"
                "FOREIGN KEY (session_id) REFERENCES oauth (session_key)"
                "ON DELETE CASCADE)");

        // Schema for prices table
        execute("CREATE TABLE IF NOT EXISTS prices"
                "(name TEXT PRIMARY KEY,"
                "unit_price REAL UNIQUE DEFAULT NULL)");
    }

    Database::~Database() {
        sqlite3_close(handle_);
    }

    void Database::execute(const std::string& query) {
        char* error_msg;
        int result{
            sqlite3_exec(handle_, query.c_str(), &callback, NULL, &error_msg)
        };
        if (result != SQLITE_OK) {
            // Temporary storage for the error
            std::string error{ error_msg };
            // Let sqlite clean up char* error_msg
            sqlite3_free(error_msg);

            throw axel::Exception(error);
        }
    }

    std::unordered_map<std::string, std::variant<std::string, int, double>>
    Database::select_row(const std::string& table,
                         const std::string& primary_key_value,
                         std::vector<std::string> attributes) {
        TableInfo table_info = get_table_info(table);
        std::string primary_key{ table_info.first };

        std::unordered_map<std::string, std::variant<std::string, int, double>>
        result{};

        // Prepare query
        std::string comma_separated_attributes{
            fmt::to_string(fmt::join(attributes, ", "))
        };
        sqlite3_stmt* statement;
        std::string query{ fmt::format("SELECT {0} FROM {1} WHERE {2} = {3}",
                                       comma_separated_attributes,
                                       table,
                                       primary_key,
                                       primary_key_value) };

        spdlog::info("{}", query);

        if (prepare(&statement, query) == 0) {
            while (sqlite3_step(statement) == SQLITE_ROW) {
                int n_columns{ sqlite3_column_count(statement) };
                auto row = extract_row(statement,
                                       attributes,
                                       table_info.second);

                for (int i{ 0 }; i < attributes.size(); i++) {
                    /// attributes
                    /// ┌───┬───┬───┬───┐
                    /// │   │   │   │   │
                    /// └───┴───┴───┴───┘
                    ///       ▲
                    ///       i
                    /// row   ▼
                    /// ┌───┬───┬───┬───┐
                    /// │   │   │   │   │
                    /// └───┴───┴───┴───┘

                    auto name = attributes.at(i);
                    auto item = row.at(i);

                    result.emplace(std::move(name), std::move(item));
                }
            }
        }

        spdlog::info("Results from SELECT query");
        for (const auto& [key, value] : result) {
            std::visit(
            [&key](const auto& val) {
                spdlog::info("Key: {}, Value: {}", key, val);
            },
            value);
        }
        return result;
    }

    TableInfo Database::get_table_info(const std::string& table) noexcept {
        std::unordered_map<std::string, std::string> map{};
        std::string primary_key;

        // Prepare query
        sqlite3_stmt* statement;
        std::string query{ fmt::format("PRAGMA table_info({0})", table) };
        if (prepare(&statement, query) == 0) {
            int count{ 0 };
            while (sqlite3_step(statement) == SQLITE_ROW) {
                // https://www.sqlite.org/pragma.html#pragma_table_info
                // Index 0 -> cid
                // Index 1 -> name
                // Index 2 -> type
                // Constructor for std::string only takes const char*
                const char* attribute_name =
                reinterpret_cast<const char*>(sqlite3_column_text(statement,
                                                                  1));

                if (count == 0) {
                    primary_key = attribute_name;
                };

                const char* type =
                reinterpret_cast<const char*>(sqlite3_column_text(statement,
                                                                  2));

                map.emplace(std::move(attribute_name), std::move(type));
                count += 1;
            }
        }
        sqlite3_finalize(statement);
        return { primary_key, map };
    }

    int Database::prepare(sqlite3_stmt** statement,
                          const std::string& query) noexcept {
        if (sqlite3_prepare_v2(handle_,
                               query.c_str(),
                               static_cast<int>(query.size() + 1),
                               statement,
                               NULL) != SQLITE_OK) {
            spdlog::warn("Unable to prepare statement: {}", query);
            return 1;
        }
        return 0;
    }

    std::vector<std::variant<std::string, int, double>>
    Database::extract_row(sqlite3_stmt* statement,
                          const std::vector<std::string>& attributes,
                          const std::unordered_map<std::string, std::string>&
                          table_info) noexcept {
        int n_columns{ sqlite3_column_count(statement) };
        std::vector<std::variant<std::string, int, double>> row;

        for (int i{ 0 }; i < n_columns; i++) {
            // Get current attribute's type
            std::string current_attribute{ attributes.at(i) };
            std::string type{ table_info.at(current_attribute) };

            // Retrieve from statement as the right type
            std::variant<std::string, int, double> item;
            if (type == "TEXT") {
                item =
                reinterpret_cast<const char*>(sqlite3_column_text(statement,
                                                                  i));

            } else if (type == "INTEGER") {
                item = static_cast<int>(sqlite3_column_int64(statement, i));
            }

            // Add to results list
            row.emplace_back(std::move(item));
        }

        return row;
    }

    int Database::delete_row(const std::string& table,
                             const std::string& attribute,
                             const std::variant<std::string, int, double>&
                             value) noexcept {
        sqlite3_stmt* statement;

        std::string query;
        std::visit(
        [&](auto&& variant) {
            using T = std::decay_t<decltype(variant)>;  // Type of value
            if constexpr (std::is_same_v<T, int>) {
                query = fmt::format("DELETE FROM {0} "
                                    "WHERE {1} = {2}",
                                    table,
                                    attribute,
                                    variant);
            } else if constexpr (std::is_same_v<T, std::string>) {
                query = fmt::format("DELETE FROM {0} "
                                    "WHERE {1} = '{2}'",
                                    table,
                                    attribute,
                                    variant);
            }
        },
        value);

        if (prepare(&statement, query) == 0) {
            int result = { sqlite3_step(statement) };
            // Clean up occurs regardless of failure or success
            sqlite3_finalize(statement);

            if (result != SQLITE_DONE) {
                spdlog::warn("Could not delete: {}", query);
                return 1;
            } else {
                spdlog::info("{}", query);
                return 0;
            }
        } else {
            return 1;
        }
    }

    int Database::update_row(const std::string& table,
                             const std::string& primary_key_value,
                             const std::string& attribute,
                             std::variant<std::string, int, double>
                             new_value) noexcept {
        auto table_info = get_table_info(table);
        auto primary_key{ table_info.first };

        // Prepare query

        sqlite3_stmt* statement;
        std::string query;

        std::visit(
        [&](auto&& variant) {
            using T = std::decay_t<decltype(variant)>;  // Type of value
            if constexpr (std::is_same_v<T, int>) {
                query = {
                    fmt::format("UPDATE {0} SET {1} = {2} WHERE {3} = {4}",
                                table,
                                attribute,
                                variant,
                                primary_key,
                                primary_key_value)
                };
            } else if constexpr (std::is_same_v<T, std::string>) {
                query = {
                    fmt::format("UPDATE {0} SET {1} = '{2}' WHERE {3} = {4}",
                                table,
                                attribute,
                                variant,
                                primary_key,
                                primary_key_value)
                };
            }
        },
        new_value);

        if (prepare(&statement, query) == 0) {
            int result = { sqlite3_step(statement) };
            // Clean up occurs regardless of failure or success
            sqlite3_finalize(statement);

            if (result != SQLITE_DONE) {
                spdlog::warn("Could not update: {}", query);
                return 1;
            } else {
                spdlog::info("{}", query);
                return 0;
            }
        } else {
            return 1;
        }
    }

    int
    Database::insert_row(const std::string& table,
                         const std::variant<database::OAuthTable,
                                            database::AppTable,
                                            database::PricesTable>& values) {
        return std::visit(
        [this, &table](auto&& variant) { return this->insert_row(table, variant); },
        values);
    }

}  // namespace util
