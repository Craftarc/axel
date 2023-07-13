#include <iostream>
#include <webutil/PkceManager.h>
#include "webutil/OauthManager.h"

int main() {
    std::cout << "All targets rebuilt" << std::endl;
    webutil::PkceManager pcke_manager;
    webutil::OauthManager var(pcke_manager);
    std::cout << var.get_state_hash() << std::endl;
}