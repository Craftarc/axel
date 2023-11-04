#ifndef AXEL_UTIL_MOCKDATABASE_H
#define AXEL_UTIL_MOCKDATABASE_H

#include <string>

#include <gmock/gmock.h>

#include "axel/Exception.h"
#include "config/database.h"
#include "util/interfaces/IDatabase.h"

#include <any>

namespace util {
    class MockDatabase : public IDatabase {
        public:
            MOCK_METHOD(
            (std::unordered_map<std::string,
                                std::variant<std::string, int, double>>),
            select_row,
            (const std::string& table,
             const std::string& primary_key_value,
             std::vector<std::string> attributes),
            (override));

            MOCK_METHOD(int,
                        insert_row,
                        (const std::string& table,
                         (const std::variant<database::OAuthTable,
                                             database::AppTable,
                                             database::PricesTable>& values)),
                        (override));

            MOCK_METHOD(int,
                        delete_row,
                        (const std::string& table,
                         const std::string& attribute,
                         (const std::variant<std::string, int, double>& value)),
                        (override, noexcept));

            MOCK_METHOD(int,
                        update_row,
                        (const std::string& table,
                         const std::string& primary_key_value,
                         const std::string& attribute,
                         (std::variant<std::string, int, double> new_value)),
                        (override, noexcept));
    };
}  // namespace util

#endif  // AXEL_UTIL_MOCKDATABASE_H
