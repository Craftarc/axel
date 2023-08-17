#ifndef AXEL_AUTH_ISESSIONMANAGER_H
#define AXEL_AUTH_ISESSIONMANAGER_H

#include <string>

namespace auth {
    class ISessionManager {
    public:
        virtual ~ISessionManager() = default;
        
        virtual std::string get_session_token() const = 0;
    };
}

#endif // AXEL_AUTH_ISESSIONMANAGER_H