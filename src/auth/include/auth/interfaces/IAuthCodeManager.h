#ifndef AXEL_AUTH_IAUTHCODEMANAGER_H
#define AXEL_AUTH_IAUTHCODEMANAGER_H

namespace auth {
    class IAuthCodeManager {
    public:
        virtual ~IAuthCodeManager() = default;
        
        virtual std::string get_auth_url(std::string code_challenge, std::string state_hash) const = 0;
    };
} // auth

#endif // AXEL_AUTH_IAUTHCODEMANAGER_H