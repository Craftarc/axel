#include <iostream>
#include <webutil/PkceManager.h>
#include "webutil/OauthManager.h"

int main() {
    std::cout << "All targets rebuilt" << std::endl;
    webutil::OauthManager var;
    var.get_authorization_url();
}