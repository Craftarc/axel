#ifndef AXEL_AXEL_IPRICES_H
#define AXEL_AXEL_IPRICES_H

#include <string>
#include <unordered_map>

namespace axel {
    /// @brief Manages pricing information from POE Ninja
    class IPrices {
        public:
            ~IPrices() = default;

            /// @brief Gets a specifiied pricelist.
            /// @param type_path Path of the pricelist to get from.
            /// @return Hash map of item names and their unit prices.
            virtual std::unordered_map<std::string, double>
            get_prices(const std::string& type_path) = 0;

            /// @brief Updates the prices database.
            /// @return Exit code.
            virtual int update_prices() = 0;

    };
}  // namespace axel

#endif
