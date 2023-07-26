#include "axel/TestHandler.h"
#include "boost/json.hpp"

namespace {
    std::string handler(const invocation_request& req) {
        std::cout << std::endl;
        std::cout << "///////// PAYLOAD /////////" << std::endl;
        std::cout << req.payload << std::endl;
        std::cout << "///////// END PAYLOAD /////////" << std::endl;
        std::cout << std::endl;
        
        boost::json::value json_value = boost::json::parse(req.payload);
        auto raw_query_string = json_value.at("rawQueryString");
        std::cout << raw_query_string << std::endl;
        
        return "";
    }
    
    void run_handler(std::istream& input) {
        invocation_request request;
        std::getline(input, request.payload);
        std::getline(input, request.request_id);
        std::getline(input, request.xray_trace_id);
        std::getline(input, request.client_context);
        std::getline(input, request.cognito_identity);
        std::getline(input, request.function_arn);
        
        // For the deadline, Need to read into a std::time_t or a string and then convert
        // Assuming you will get seconds since epoch from input and convert it
        std::time_t deadline;
        if (input >> deadline) {
            request.deadline = std::chrono::system_clock::from_time_t(deadline);
        }
        
        input.ignore();  // Ignore newline after time
    }
}

namespace axel {
    void TestHandler::run() const {
        std::cout << "HI" << std::endl;
    }
};

