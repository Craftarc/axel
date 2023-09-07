#include "poe_ninja/get_poe_ninja_data.h"

#include <string>

#include "config/poe_ninja_config.h"
#include "util/HttpSender.h"
#include "util/http.h"

namespace poe_ninja {
	using namespace util;  // TODO: Document this namespace so get_item_prices gets documented as well

	/// Sends an HTTP GET request to given host and path, retrieve response as a string.
	///
	/// @param resource_path - Path to specific data resource at host.
	///
	/// @return - Response body from GET request as string.
	std::string get_item_prices(const std::string& resource_path) {
		auto request = make_http_request("GET", resource_path, { { "host", config::poe_ninja::host } });

		return HttpSender().send_http_request(request, 15);  // TODO: Remove magic number
	}
}  // namespace poe_ninja
