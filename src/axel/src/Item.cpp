#include "axel/Item.h"

#include "util/type.h"

namespace axel {
	// Public
	/// An object that stores information about a specific item class in the player's stashes.
	/// @param name Name of the item.
	/// @param quantity Total quantity of the item present in the player's stashes.
	/// @param unit_price Unit price of the item, in Chaos Orbs.
	Item::Item(std::string name, int64_t quantity, double unit_price) :
		name_{ std::move(name) },
		quantity_{ quantity },
		unit_price_{ unit_price },
		total_price_{ unit_price * util::int64_t_to_double(quantity) } {}

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