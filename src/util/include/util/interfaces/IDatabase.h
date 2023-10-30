#ifndef AXEL_UTIL_DATABASE_H
#define AXEL_UTIL_DATABASE_H

#include <string>
#include <unordered_map>
#include <variant>
#include <vector>

#include "config/database.h"

namespace util {
    class IDatabase {
        public:
            virtual ~IDatabase() = default;

            virtual std::unordered_map<std::string,
                                       std::variant<std::string, int>>
            select_row(const std::string& table,
                       const std::string& primary_key_value,
                       std::vector<std::string> attributes) = 0;

            // Interface for database::AppTable version of insert_row
            // For testing purposes
            virtual int insert_row_apptable(const std::string& table,
                                   const database::AppTable& values);

            // Interface for database::OAuthTable version of insert_row
            // For testing purposes
            virtual int insert_row_oauthtable(const std::string& table,
                                   const database::OAuthTable& values);

            virtual int
            delete_row(const std::string& table,
                       const std::string& attribute,
                       const std::variant<std::string, int>& value) noexcept;
    };

}  // namespace util

#endif  // AXEL_UTIL_DATABASE_H
