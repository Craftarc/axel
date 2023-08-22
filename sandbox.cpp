#include <iostream>
#include "axel/UpdateManager.h"

int main() {
    std::string token = "ffab4d82ee3a2e274aee626cb64e1a1904729173";
    axel::UpdateManager update_manager(token);
    auto request = update_manager.make_stash_request();
    std::cout << request.base()["authorization"] << std::endl;
}