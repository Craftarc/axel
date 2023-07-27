#include <string>
#include <aws/lambda-runtime/runtime.h>
#include <iostream>
#include <fstream>
#include <boost/json.hpp>

#include "parse/string.h"

int main() {
    std::ifstream file{"test_input/axel/response.json", std::ios_base::in};
    std::string f = parse::read_file_into_string(file);
    auto request = parse::make_invocation_request(f);
    
    std::cout << request.request_id << std::endl;
    
    return 0;
}