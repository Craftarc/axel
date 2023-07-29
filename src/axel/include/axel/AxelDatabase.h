#ifndef AXEL_AXEL_AXELDATABASE_H
#define AXEL_AXEL_AXELDATABASE_H

#include "interface/IDatabase.h"

namespace database {
    class AxelDatabase : public IDatabase {
        void store(const std::vector<std::string>& items) const;
    };
}

#endif //AXEL_AXEL_AXELDATABASE_H
