#ifndef AXEL_AXEL_ITEM_H
#define AXEL_AXEL_ITEM_H

#include <string>

#include <boost/json.hpp>

namespace json = boost::json;

namespace axel {
    class Item {
        public:
            /// @brief Default constructor.
            /// An object that stores information about a
            /// specific item class in the player's stashes.
            /// @param name Name of the item.
            /// @param quantity Total quantity of the item present in the player's stashes.
            /// @param unit_price Unit price of the item, in Chaos Orbs.
            Item(int id, std::string name, int64_t quantity, double unit_price);

            /// @brief Get the unique ID of the item.
            int get_id() const;

            /// @brief Get the name of the item.
            const std::string& get_name() const;

            /// @brief Get the quantity of this item in the player's stashes.
            int64_t get_quantity() const;

            /// @brief Get the price, in Chaos Orbs, of one unit of this item.
            double get_unit_price() const;

            /// @brief Get the total price, in Chaos Orbs, of all units
            /// of this item in the player's stashes.
            double get_total_price() const;

        private:
            int id_;
            std::string name_;
            int64_t quantity_;
            double unit_price_;
            double total_price_;
    };
}  // namespace axel

#endif  // AXEL_AXEL_ITEM_H
