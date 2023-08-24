#include <iostream>
#include "axel/UpdateManager.h"
#include "webutil/HttpSender.h"

int main() {
    std::string token = "96b8208784efb82bb63f802269512465ad046b45";
    axel::UpdateManager update_manager(token);
    auto request = update_manager.make_stash_request();
    std::cout << request << std::endl;
    auto response = webutil::HttpSender().send_http_request(request, 15);
    std::cout << response << std::endl;
}