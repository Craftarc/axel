#include <aws/core/Aws.h>
#include "axel/Handler.h"

int main() {
    Aws::SDKOptions options;
    Aws::InitAPI(options);
    axel::Handler::run();
    Aws::ShutdownAPI(options);
    return 0;
}