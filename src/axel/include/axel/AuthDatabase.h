#ifndef AXEL_AXEL_AUTHDATABASE_H
#define AXEL_AXEL_AUTHDATABASE_H

#include "interface/IDatabase.h"

class AuthDatabase : public IDatabase {
public:
    void store(const std::vector<std::string>& items) const;
};

#endif //AXEL_AXEL_AUTHDATABASE_H
