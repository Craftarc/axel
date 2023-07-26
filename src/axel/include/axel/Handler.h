#ifndef AXEL_HANDLER_H
#define AXEL_HANDLER_H

#include "axel/interfaces/IHandler.h"

namespace axel {
    class Handler : public IHandler {
    public:
        /// @brief Runs the handler implementation that communicates with the AWS Lambda server runtime
        void run() const override;
    };
}

#endif // AXEL_HANDLER_H