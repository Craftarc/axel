#ifndef AXEL_AUTH_ISTATEHASHMANAGER_H
#define AXEL_AUTH_ISTATEHASHMANAGER_H
namespace auth {
    class IStateHashManager {
    public:
        virtual ~IStateHashManager() = default;
        virtual std::string get_state_hash() const = 0;
        virtual bool check_state_hash(const std::string& check_hash) const = 0;
    private:
        virtual bool store_state_hash(const std::string& input_hash) const = 0;
    };
}

#endif // AXEL_AUTH_ISTATEHASHMANAGER_H