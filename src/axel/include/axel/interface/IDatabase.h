#ifndef AXEL_AXEL_IDATABASE_H
#define AXEL_AXEL_IDATABASE_H

#include <vector>
#include <string>

class IDatabase {
public:
    virtual ~IDatabase();
    /// @brief Stores items in the database
    void store(const std::vector<std::string>& items) const;
};

#endif //AXEL_AXEL_IDATABASE_H
