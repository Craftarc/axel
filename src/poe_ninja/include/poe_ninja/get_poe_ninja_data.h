#ifndef AXEL_POE_NINJA_GET_POE_NINJA_DATA_H
#define AXEL_POE_NINJA_GET_POE_NINJA_DATA_H

#include <memory>
#include <string>

#include "util/interfaces/IHttpSender.h"

namespace poe_ninja {
	/// @brief Fetches data from POE Ninja using specified host and path.
	std::string get_item_prices(const std::string& resource_path,
	                            const std::shared_ptr<util::IHttpSender>& http_sender);
}  // namespace poe_ninja
#endif  // AXEL_POE_NINJA_GET_POE_NINJA_DATA_H