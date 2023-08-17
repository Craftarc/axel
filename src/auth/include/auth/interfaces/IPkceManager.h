#ifndef AXEL_AUTH_IPKCEMANAGER_H
#define AXEL_AUTH_IPKCEMANAGER_H

namespace auth {
    class IPkceManager {
    public:
        virtual ~IPkceManager() = default;
        
        virtual std::string get_code_verifier() const = 0;
        
        virtual std::string get_code_challenge() const = 0;
    };
}

#endif // AXEL_AUTH_IPKCEMANAGER_H