#include "poe_ninja/get_poe_ninja_data.h"

#include <string>

#include <spdlog/spdlog.h>

#include "axel/Config.h"
#include "util/http.h"
#include "util/interfaces/IHttpSender.h"

namespace poe_ninja {
    using namespace util;  // TODO: Document this namespace so get_item_prices gets documented as well

    /// Sends an HTTP GET request to given host and path, retrieve response as a string.
    ///
    /// @param resource_path - Path to specific data resource at host.
    ///
    /// @return - Response body from GET request as string.
    std::string
    get_item_prices(const std::string& resource_path,
                    const std::shared_ptr<IHttpSender>& http_sender) {
        auto request = make_http_request("GET",
                                         resource_path,
                                         { { "host",
                                             CONFIG("api.host.poe_ninja") } });

        return http_sender->send_http_request(request,
                                              15);  // TODO: Remove magic number
    }
}  // namespace poe_ninja
