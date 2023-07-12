#ifndef AXEL_PATHUTIL_H
#define AXEL_PATHUTIL_H

#include <string>
#include <map>

namespace pathutil {
    std::string
    add_query_parameters(const std::string& url, const std::map<std::string, std::string>& parameters);
}

#endif //AXEL_PATHUTIL_H
