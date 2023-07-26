#include <string>
#include <aws/lambda-runtime/runtime.h>
#include <iostream>
#include <fstream>
#include <boost/json.hpp>

using namespace aws::lambda_runtime;

invocation_request get_input(std::istream& input) {
    
    std::string string;
    char character;
    while (input.get(character)) {
        string += character;
    }
    
    std::cout << string << std::endl;
    return invocation_request{};
}

int main() {
    std::ifstream file{"test_input/axel/response.json", std::ios_base::in};
    get_input(file);
    return 0;
}