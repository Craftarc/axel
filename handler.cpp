#include <aws/lambda-runtime/runtime.h>
#include <iostream>

using namespace aws::lambda_runtime;

static invocation_response my_handler(invocation_request const& req) {
    
    std::cout << req.payload << std::endl;
    
    if (req.payload.length() > 42) {
        return invocation_response::failure("error message here"/*error_message*/,
                                            "error type here" /*error_type*/);
    }
    
    return invocation_response::success("json payload here" /*payload*/,
                                        "application/json" /*MIME type*/);
}

int main() {
    run_handler(my_handler);
    return 0;
}