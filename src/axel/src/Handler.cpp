#include <aws/lambda-runtime/runtime.h>
#include <iostream>
#include <boost/json.hpp>
#include "axel/Handler.h"

using namespace aws::lambda_runtime;
namespace {
    /// @brief Takes a request from the AWS Lambda server runtime and delegates it to the appropriate method to be processed.
    invocation_response handler(const invocation_request& req) {
        std::cout << std::endl;
        std::cout << "///////// PAYLOAD /////////" << std::endl;
        std::cout << req.function_arn << std::endl;
        std::cout << "///////// END PAYLOAD /////////" << std::endl;
        std::cout << std::endl;

//        boost::json::value json_value = boost::json::parse(req.payload);
//        auto raw_query_string = json_value.at("rawQueryString");
//        std::cout << raw_query_string << std::endl;
        
        return invocation_response::success("" /*payload*/,
                                            "application/json" /*MIME type*/);
    }
}
namespace axel {
    void Handler::run() const {
        aws::lambda_runtime::run_handler(handler);
    }
}
