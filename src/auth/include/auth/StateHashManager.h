#ifndef AXEL_STATEHASHMANAGER_H
#define AXEL_STATEHASHMANAGER_H

#include "auth/interfaces/IStateHashManager.h"
#include <string>

namespace auth {
    /// @brief The StateHashManager class.
    ///
    /// The StateHashManager class is responsible for generating and storing the state hash, which is used to track
    /// the current authorization request. This is in order to prevent Cross-Site Request Forgery.
    ///
    /// The response containing the authorization code from the authorization server should be verified against the one
    /// stored in an instance of this class.
    ///
    /// @note There should only be one instance of this class per Oauth session.
    class StateHashManager : public IStateHashManager {
    public:
        
        /// @brief Gets the state hash.
        [[nodiscard]] std::string get_state_hash() const override;
        
        /// @brief Checks if an input state hash matches the one associated with the same authentication session.
        bool check_state_hash(std::string session_id, std::string state_hash) const override;
    };
}

#endif //AXEL_STATEHASHMANAGER_H
