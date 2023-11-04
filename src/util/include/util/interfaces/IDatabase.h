#ifndef AXEL_UTIL_IDATABASE_H
#define AXEL_UTIL_IDATABASE_H
#include <string>
#include <unordered_map>
#include <variant>
#include <vector>

#include "config/database.h"

#include <any>

namespace util {
    class IDatabase {
        public:
            virtual ~IDatabase() = default;

            virtual int
            insert_row(const std::string& table,
                       const std::variant<database::OAuthTable,
                                          database::AppTable,
                                          database::PricesTable>& values) = 0;

            virtual std::unordered_map<std::string,
                                       std::variant<std::string, int, double>>
            select_row(const std::string& table,
                       const std::string& primary_key_value,
                       std::vector<std::string> attributes) = 0;

            virtual int delete_row(const std::string& table,
                                   const std::string& attribute,
                                   const std::variant<std::string, int, double>&
                                   value) noexcept = 0;

            virtual int update_row(const std::string& table,
                                   const std::string& primary_key_value,
                                   const std::string& attribute,
                                   std::variant<std::string, int, double>
                                   new_value) noexcept = 0;
    };

}  // namespace util

#endif  // AXEL_UTIL_IDATABASE_H
