#include "axel/Item.h"

#include "util/type.h"

namespace {
    // unit_price will be -1 if it was invalid (ie. it could not be found
    // in price lists fetched from poe.ninja)
    // In that case we also set total_price to -1 to represent an invalid total
    // price
    double calculate_total_price(int64_t quantity, double unit_price) {
        if (unit_price == -1) {  // Unit price is invalid
            return -1;
        } else {
            return quantity * unit_price;
        }
    }
}  // namespace

namespace axel {
    // Public
    Item::Item(int id, std::string name, int64_t quantity, double unit_price) :
        id_{ id },
        name_{ std::move(name) },
        quantity_{ quantity },
        unit_price_{ unit_price },
        total_price_{ calculate_total_price(quantity, unit_price) } {}

    int Item::get_id() const {
        return id_;
    }

    const std::string& Item::get_name() const {
        return name_;
    }

    int64_t Item::get_quantity() const {
        return quantity_;
    }

    double Item::get_unit_price() const {
        return unit_price_;
    }

    double Item::get_total_price() const {
        return total_price_;
    }

}  // namespace axel
