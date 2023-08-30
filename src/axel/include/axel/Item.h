#ifndef AXEL_AXEL_ITEM_H
#define AXEL_AXEL_ITEM_H

#include <string>

namespace axel {
    class Item {
    public:
        /// @brief Default constructor.
        Item(std::string name, int quantity, double unit_price);
        
        /// @brief Get the name of the item.
        [[nodiscard]] const std::string& get_name() const;
        /// @brief Get the quantity of this item in the player's stashes.
        [[nodiscard]] int get_quantity() const;
        /// @brief Get the price, in Chaos Orbs, of one unit of this item.
        [[nodiscard]] double get_unit_price() const;
        /// @brief Get the total price, in Chaos Orbs, of all units of this item in the player's stashes.
        [[nodiscard]] double get_total_price() const;
    private:
        const std::string name_;
        const int quantity_;
        const double unit_price_;
        const double total_price_;
    };
}

#endif // AXEL_AXEL_ITEM_H