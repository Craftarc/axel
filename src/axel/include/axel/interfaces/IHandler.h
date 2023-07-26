#ifndef AXEL_AXEL_IHANDLER_H
#define AXEL_AXEL_IHANDLER_H

#include <aws/lambda-runtime/runtime.h>

using namespace aws::lambda_runtime;
namespace axel {
    class IHandler {
    public:
        virtual ~IHandler() = default;
        virtual void run() const = 0;
    };
} // axel

#endif //AXEL_AXEL_IHANDLER_H