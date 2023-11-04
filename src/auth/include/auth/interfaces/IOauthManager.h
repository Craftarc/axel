#ifndef AXEL_IOAUTHMANAGER_H
#define AXEL_IOAUTHMANAGER_H

#include <string>

#include <crow.h>

namespace auth {
    class IOauthManager {
        public:
            virtual ~IOauthManager() = default;

            virtual void start_auth(crow::response& response) = 0;
            virtual void receive_auth(const std::string& query_string,
                                      const std::string& session_id,
                                      crow::response& response) = 0;
    };
}  // namespace auth

#endif  //AXEL_IOAUTHMANAGER_H
