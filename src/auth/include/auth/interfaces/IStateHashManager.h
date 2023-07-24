#ifndef AXEL_ISTATEHASHMANAGER_H
#define AXEL_ISTATEHASHMANAGER_H
namespace auth {
    class IStateHashManager {
    public:
        virtual ~IStateHashManager() = default;
        virtual std::string get_state_hash() const = 0;
        virtual bool check_state_hash(const std::string& check_hash) const = 0;
    };
}

#endif // AXEL_ISTATEHASHMANAGER_H