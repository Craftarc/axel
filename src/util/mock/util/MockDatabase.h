#ifndef AXEL_UTIL_MOCKDATABASE_H
#define AXEL_UTIL_MOCKDATABASE_H

#include <gmock/gmock.h>

#include "axel/Exception.h"
#include "util/interfaces/IDatabase.h"

namespace util {
    class MockDatabase : public IDatabase {
        public:
            MOCK_METHOD((std::unordered_map<std::string,
                                            std::variant<std::string, int>>),
                        select_row,
                        (const std::string& table,
                         const std::string& primary_key_value,
                         std::vector<std::string> attributes),
                        (override));

            MOCK_METHOD(int,
                        insert_row_apptable,
                        (const std::string& table,
                         const database::AppTable& values),
                        (override));

            MOCK_METHOD(int,
                        insert_row_oauthtable,
                        (const std::string& table,
                         const database::OAuthTable& values),
                        (override));

            MOCK_METHOD(int,
                        delete_row,
                        (const std::string& table,
                         const std::string& attribute,
                         (const std::variant<std::string, int>& value)),
                        (override, noexcept));

            // Dispatches to the right MOCK_METHOD
            template<typename T>
            int insert_row(const std::string& table, const T& values) {
                throw axel::Exception("Unexpected call to dispatcher method");
            }

            // OAuthTable specialisation
            template<>
            int insert_row(const std::string& table,
                           const database::OAuthTable& values) {
                insert_row_oauthtable(table, values);
            }

            // AppTable specialisation
            template<>
            int insert_row(const std::string& table,
                           const database::AppTable& values) {
                insert_row_apptable(table, values);
            }
    };
}  // namespace util

#endif  // AXEL_UTIL_MOCKDATABASE_H
