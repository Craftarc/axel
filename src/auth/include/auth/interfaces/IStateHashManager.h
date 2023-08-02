#ifndef AXEL_AUTH_ISTATEHASHMANAGER_H
#define AXEL_AUTH_ISTATEHASHMANAGER_H
namespace auth {
    class IStateHashManager {
    public:
        virtual ~IStateHashManager() = default;
        virtual std::string get_state_hash() const = 0;
        virtual bool check_state_hash(std::string session_id, std::string state_hash) const = 0;
    };
}

#endif // AXEL_AUTH_ISTATEHASHMANAGER_H