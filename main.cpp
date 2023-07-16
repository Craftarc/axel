#include <iostream>
#include "webutil/httputil.h"

int main() {
    std::cout << "All targets rebuilt" << std::endl;
    std::cout << webutil::send_http_request(nullptr) << std::endl;
}