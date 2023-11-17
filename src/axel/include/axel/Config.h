#ifndef AXEL_AXEL_CONFIG_H
#define AXEL_AXEL_CONFIG_H

#define CONFIG(path) axel::Config::get(path)

#include <string>

#include <boost/json.hpp>

namespace json = boost::json;

namespace axel {
    class Config {
        public:
            /// @brief Retrieves the value specified from the config.json
            /// configuration file.
            /// @param key Dot-separated representation of the property to
            /// retrieve a value for. (eg. auth.poe.host)
            /// @return the Value of the requested property.
            static std::string get(const std::string& key);

        private:
            /// @brief Constructor.
            /// @param config_path Path to the configuration file.
            Config(const std::string& config_path);

            /// @brief Gets the singular instance of the Config object.
            /// @note Config is a singleton.
            static Config& get_instance();

            /// @brief Gets the stored boost::json::value from the singleton
            /// Config.
            static json::value get_config();

            json::value config_;
    };

}  // namespace axel

#endif  // AXEL_AXEL_CONFIG_H
