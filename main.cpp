#include <iostream>
#include "webutil/http.h"
#include "src/auth/include/OauthManager.h"

int main() {
    webutil::OauthManager oauth_manager;
    std::cout << oauth_manager.get_authorization_url();
}