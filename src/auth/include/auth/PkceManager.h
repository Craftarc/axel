#ifndef AXEL_AUTH_PKCEMANAGER_H
#define AXEL_AUTH_PKCEMANAGER_H

#include <string>

#include "interfaces/IPkceManager.h"

namespace auth {
    /// @brief Responsible for managing Pkce (Proof Key for Code Exchange) code verifiers and challenges.
    ///
    /// There should only be one Pkce proof pair per authentication session.
    class PkceManager : public IPkceManager {
    public:
        /// @brief Constructs a new PkceManager object.
        PkceManager();
        
        /// @brief Gets the Code Verifier.
        [[nodiscard]] std::string get_code_verifier() const override;
        
        /// @brief Gets the Code Challenge.
        [[nodiscard]] std::string get_code_challenge() const override;
    private:
        std::string code_verifier_;
        std::string code_challenge_;
    };
}

#endif //AXEL_AUTH_PKCEMANAGER_H
