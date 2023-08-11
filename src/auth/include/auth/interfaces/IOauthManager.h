#ifndef AXEL_IOAUTHMANAGER_H
#define AXEL_IOAUTHMANAGER_H

#include <string>

namespace auth {
    class IOauthManager {
    public:
        virtual ~IOauthManager() = default;
        
        virtual std::string start_auth() = 0;
        virtual std::string
        receive_auth(const std::string& query_string,
                     const std::string& session_id,
                     int64_t request_time) = 0;
    };
}

#endif //AXEL_IOAUTHMANAGER_H
