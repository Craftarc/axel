#include <iostream>
#include "auth/TokenRequestManager.h"

int main() {
    auth::TokenRequestManager token_request_manager;
    std::string auth_code = "1234";
    std::string code_verifier = "5678";
    auto pair = token_request_manager.send_token_request(auth_code, code_verifier, nullptr);
}