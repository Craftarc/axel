#ifndef AXEL_AXEL_TESTHANDLER_H
#define AXEL_AXEL_TESTHANDLER_H

#include <gmock/gmock.h>
#include <aws/lambda-runtime/runtime.h>

using namespace aws::lambda_runtime;
namespace axel {
    class TestHandler {
    public:
        /// @brief Runs the handler implementation that will process local input (file or std::cin)
        static invocation_response run(const invocation_request& request);
    };
}

#endif // AXEL_AXEL_TESTHANDLER_H
