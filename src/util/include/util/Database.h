#ifndef AXEL_UTIL_DATABASE_H
#define AXEL_UTIL_DATABASE_H

#include <string>
#include <unordered_map>
#include <variant>
#include <vector>

#include <fmt/core.h>
#include <spdlog/spdlog.h>
#include <sqlite3.h>

#include "axel/Exception.h"
#include "config/database.h"

#include <map>

namespace util {
    /// @brief Database handler object
    /// Wrapper interface for an SQLite managed database
    class Database {
        public:
            /// @brief Constructor.
            /// @param name Filename of the databse
            Database(const char* name);

            ~Database();
            Database(const Database&) = delete;
            Database& operator=(const Database&) = delete;
            Database(Database&&) = delete;
            Database& operator=(Database&&) = delete;

            /// @brief Selects rows from a given table.
            /// @param table Name of table to select from.
            /// @param primary_key Value of primary key to search for.
            /// @param attributes List of attributes to retrieve from the table.
            /// The first item must be the primary key.
            /// @return Hash table of the attributes and their respective values.
            /// Empty hash table if no row found.
            std::unordered_map<std::string, std::variant<std::string, int>>
            select_row(const std::string& table,
                       const std::string& primary_key_value,
                       std::vector<std::string> attributes);

            /// @brief Inserts a row into the app table.
            /// @param table The name of the table to insert into.
            /// @param values A set of values to insert.
            /// @return Exit code.
            /// @note Strong guarantee.
            template<typename T>
            typename std::enable_if<std::is_same<T, database::AppTable>::value,
                                    int>::type
            insert_row(const std::string& table, const T& values) {
                sqlite3_stmt* statement;
                std::string query{ fmt::format("INSERT INTO {0} "
                                               "VALUES ('{1}', '{2}', {3})",
                                               table,
                                               values.session_id,
                                               values.access_token,
                                               values.time_to_live) };

                spdlog::info("{}", query);

                if (prepare(&statement, query) == 0) {
                    int result = { sqlite3_step(statement) };
                    // Clean up occurs regardless of failure or success
                    sqlite3_finalize(statement);

                    if (result != SQLITE_DONE) {
                        spdlog::warn("Could not insert: {}", query);
                        return 1;
                    } else {
                        return 0;
                    }
                } else {
                    return 1;
                }
            }

            /// @brief Deletes a row from a table by target attribute.
            /// @param The name of the table to delete.
            /// @param attribute Name of the attribute to search for.
            /// @param value Value of the attribute to search for.
            /// @return Exit code.
            int
            delete_row(const std::string& table,
                       const std::string& attribute,
                       const std::variant<std::string, int>& value) noexcept;

            /// @brief Inserts a row into the oauth table.
            /// @param table The name of the table to insert into.
            /// @param values A set of values to insert.
            /// @return Exit code.
            /// @note Strong guarantee.
            template<typename T>
            typename std::
            enable_if<std::is_same<T, database::OAuthTable>::value, int>::type
            insert_row(const std::string& table, const T& values) {
                sqlite3_stmt* statement;
                std::string query{ fmt::format("INSERT INTO {0} "
                                               "VALUES ('{1}', '{2}', {3})",
                                               table,
                                               values.session_id,
                                               values.access_token,
                                               values.time_to_live) };

                spdlog::info("{}", query);

                if (prepare(&statement, query) == 0) {
                    int result = { sqlite3_step(statement) };
                    // Clean up occurs regardless of failure or success
                    sqlite3_finalize(statement);

                    if (result != SQLITE_DONE) {
                        spdlog::warn("Could not insert: {}", query);
                        return 1;
                    } else {
                        return 0;
                    }
                } else {
                    return 1;
                }
            }

            /// @brief Updates a row in the table
            /// @param table The name of the table where the row to update is.
            /// @param primary_key_value The primary key value of the row to update.
            /// @param attribute The target attribute of the row which will receive a new value.
            /// @param new_value The new value.
            /// @return Exit code.
            int update_row(const std::string& table,
                           const std::string& primary_key_value,
                           const std::string& attribute,
                           std::variant<std::string, int> new_value) noexcept;

        protected:
            /// @brief Executes an SQL query.
            /// @param query The SQL query to execute.
            /// @throw axel::Exception
            /// @note Strong guarantee.
            void execute(const std::string& query);

        private:
            sqlite3* handle_;

            /// @brief Gets attribute and type information of the table.
            /// @param table Name of table to get information about.
            /// @return A pair. The first element is the name of the primary key.
            /// The second element is a hash map of (attribute name, type) key-values.
            std::pair<std::string, std::unordered_map<std::string, std::string>>
            get_table_info(const std::string& table) noexcept;

            /// @brief Prepares an SQL statement from query
            /// @param statement Out-param for the prepared statement.
            /// @param query The SQL query string.
            /// @return Exit code.
            int prepare(sqlite3_stmt** statement,
                        const std::string& query) noexcept;

            /// @brief Extracts all columns of a row.
            /// @param Pointer to statement (pointing at some row after sqlite3_step())
            /// @param Ordered list of attributes that this statement will produce.
            /// @param Hash map of the table's types and their values.
            /// @return List of values of the requested attributes in the row.
            std::vector<std::variant<std::string, int>>
            extract_row(sqlite3_stmt* statement,
                        const std::vector<std::string>& attributes,
                        const std::unordered_map<std::string, std::string>&
                        table_info) noexcept;
    };
}  // namespace util

#endif  // AXEL_UTIL_DATABASE_H
