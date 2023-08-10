#include <aws/core/Aws.h>
#include <spdlog/spdlog.h>

#include "axel/Handler.h"
#include "parse/util.h"

int main() {
// Set logger output level
#ifdef NDEBUG // Release
    spdlog::set_level(spdlog::level::info);
#else
    spdlog::set_level(spdlog::level::trace);
#endif
    Aws::SDKOptions options;
    
    Aws::InitAPI(options);

#ifdef TEST
    axel::Handler handler{"test"};
    handler.run();
#else
    axel::Handler handler{"production"};

#endif
    handler.run();
    Aws::ShutdownAPI(options);
    return 0;
}