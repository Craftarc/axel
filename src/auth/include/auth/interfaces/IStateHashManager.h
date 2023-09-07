#ifndef AXEL_AUTH_ISTATEHASHMANAGER_H
#define AXEL_AUTH_ISTATEHASHMANAGER_H
#include <string>

namespace auth {
	class IStateHashManager {
		public:
			virtual ~IStateHashManager() = default;
			virtual std::string get_state_hash() const = 0;
			virtual bool check_state_hash(std::string state_hash, std::string stored_hash) const = 0;
	};
}  // namespace auth

#endif  // AXEL_AUTH_ISTATEHASHMANAGER_H