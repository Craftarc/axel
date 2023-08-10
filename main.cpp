#include <aws/core/Aws.h>
#include <spdlog/spdlog.h>

#include "axel/Handler.h"
#include "axel/TestHandler.h"
#include "parse/util.h"

int main() {
// Set logger output level
#ifdef NDEBUG // Release
    spdlog::set_level(spdlog::level::info)
#else
    spdlog::set_level(spdlog::level::trace);
#endif
    Aws::SDKOptions options;
    
    Aws::InitAPI(options);

#ifdef RIE_MOUNT
    axel::Handler::run();
#else
    invocation_request request{parse::make_invocation_request("test_input/auth/auth-response.json")};
    axel::TestHandler::run(request);
#endif
    
    Aws::ShutdownAPI(options);
    return 0;
}