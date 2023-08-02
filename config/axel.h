#ifndef AXEL_CONFIG_AXEL_H
#define AXEL_CONFIG_AXEL_H

namespace config {
    /// @brief Contains constants related to Axel's functionality
    namespace axel {
        const std::string client_id = "axel";
        const std::string grant_type = "authorization_code";
        namespace database {
            const std::string auth = "axel-oauth";
            const std::string app = "axel-app";
        }
    }
}

#endif // AXEL_CONFIG_AXEL_H