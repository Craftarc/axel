#include "axel/HandlerManager.h"
#include "axel/Handler.h"
#include "axel/TestHandler.h"

namespace axel {
    HandlerManager::HandlerManager(const std::string& environment)
            : handler_{(environment == "production")
                       ? static_cast<std::unique_ptr<IHandler>>(std::make_unique<Handler>())
                       : static_cast<std::unique_ptr<IHandler>>(std::make_unique<TestHandler>())} {
    } // TODO: Change to dynamic cast
    
    void HandlerManager::run_handler() const { handler_->run(); }
}