#ifndef AXEL_STATEHASHMANAGER_H
#define AXEL_STATEHASHMANAGER_H

#include <string>

namespace webutil {
    /**
     * @brief The StateHashManager class.
     *
     * The StateHashManager class is responsible for generating and storing the state hash, which is used to track
     * the current authorization request. This is in order to prevent Cross-Site Request Forgery.
     *
     * The response containing the authorization code from the authorization server should be verified against the one
     * stored in an instance of this class.
     *
     * @note There should only be one instance of this class per Oauth session.
     */
    class StateHashManager {
    public:
        /**
         * @brief Constructs a StateHashManager instance.
         */
        StateHashManager();
        
        /**
        * @brief Get the state hash.
        *
        * @return State hash. This is used to track the current authorization
        * request to prevent Cross-Site Request Forgery
        */
        std::string get_state_hash() const;
        
        /**
         * @brief Verifies if a given hash matches the state hash stored internally.
         *
         * @return true if the given hash matches the state hash. false otherwise.
         */
        bool verify_state_hash(const std::string& check_hash) const;
    
    private:
        std::string state_hash_;
    };
}
#endif //AXEL_STATEHASHMANAGER_H
