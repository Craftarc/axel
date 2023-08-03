#ifndef AXEL_AUTH_SESSIONMANAGER_H
#define AXEL_AUTH_SESSIONMANAGER_H

#include <string>

#include "auth/interfaces/ISessionManager.h"

namespace auth {
    class SessionManager : public ISessionManager {
    public:
        SessionManager();
        
        /// @brief Get the session token for the current user session
        std::string get_session_token() const;
    };
}

#endif //AXEL_AUTH_SESSIONMANAGER_H
