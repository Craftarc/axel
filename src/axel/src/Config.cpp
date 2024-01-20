#include "axel/Config.h"

#include <fstream>

#include <boost/json.hpp>
#include <parse/util.h>

#include "axel/Exception.h"
#include "parse/json.h"

namespace json = boost::json;

namespace axel {
    Config::Config(const std::string& config_path) {
        std::ifstream file(config_path);
        std::string json_string{ parse::read_file_into_string(file) };
        config_ = json::parse(json_string);
    }

    Config& Config::get_instance() {
        // Find the path to the configuration file
        std::string project_root{ std::getenv("APP_ROOT") };
        std::string path_to_config{ project_root + "/config/config.json" };

        static Config instance(path_to_config);
        return instance;
    }

    json::value Config::get_config() {
        return get_instance().config_;
    }

    std::string Config::get(const std::string& key) {
        json::value config = Config::get_config();
        parse::JsonResult value =
        parse::get<json::string>(config, parse::dot_to_jsonptr(key));
        if (value.is_success()) {
            return value.get().c_str();
        } else {
            throw Exception("Unable to get configuration value");
        }
    }

}  // namespace axel
