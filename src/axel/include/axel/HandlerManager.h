#include <memory>
#include "axel/interfaces/IHandler.h"

namespace axel {
    class HandlerManager {
    public:
        /// @brief Default constructor. Sets the type of handler to use for processing incoming requests.
        HandlerManager(const std::string& environment);
        
        /// @brief Runs the handler
        void run_handler() const;
    
    private:
        std::unique_ptr<IHandler> handler_;
    };
} // axel