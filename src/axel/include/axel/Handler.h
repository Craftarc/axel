#ifndef AXEL_HANDLER_H
#define AXEL_HANDLER_H

namespace axel {
    class Handler {
    public:
        /// @brief Runs the handler implementation that communicates with the AWS Lambda server runtime
        static void run();
    };
}

#endif // AXEL_HANDLER_H