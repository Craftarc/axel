#ifndef AXEL_TESTHANDLER_H
#define AXEL_TESTHANDLER_H

#include <gmock/gmock.h>

#include "axel/interfaces/IHandler.h"

namespace axel {
    class TestHandler : public IHandler {
    public:
        /// @brief Runs the handler implementation that will process local input (file or std::cin)
        void run() const override;
    };
}

#endif // AXEL_TESTHANDLER_H
