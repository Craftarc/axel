#ifndef AXEL_PKCEMANAGER_H
#define AXEL_PKCEMANAGER_H

namespace webutil {
/**
 * @brief A class to manage PKCE (Proof Key for Code Exchange) code verifiers and challenges.
 *
 * There should only be one PKCE proof pair per authentication session.
 */
    class PKCE {
    public:
        /**
         * @brief Construct a new PKCE object.
         *
         * Generate and store a code verifier - code challenger pair
         */
        PKCE();
        
        /**
         * @brief Get the Code Verifier.
         *
         * As per RFC 7636 Section 4, the code verifier is a cryptographic random string using the characters
         * [A-Z], [a-z], [0-9], "-", ".", "_", and "~", with a length between 43 and 128 characters.
         *
         * @return std::string The code verifier string.
         */
        std::string get_code_verifier() const;
        
        /**
         * @brief Get the Code Challenge.
         *
         * The code challenge is derived from the code verifier. According to the PKCE specification,
         * as well as GGG's requirements, this function uses the method S256.
         *
         * @return std::string The code challenge string.
         */
//        std::string getCodeChallenge() const;
    
    
    private:
        std::string code_verifier_;
        std::string code_challenge_;
        
        
    };
    
} // webutil
#endif //AXEL_PKCEMANAGER_H
