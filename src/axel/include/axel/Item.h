#ifndef AXEL_AXEL_ITEM_H
#define AXEL_AXEL_ITEM_H

#include <string>

namespace axel {
	class Item {
		public:
			/// @brief Default constructor.
			Item(std::string name, int64_t quantity, double unit_price);

			/// @brief Get the name of the item.
			[[nodiscard]]
			const std::string& get_name() const;
			/// @brief Get the quantity of this item in the player's stashes.
			[[nodiscard]]
			int64_t get_quantity() const;
			/// @brief Get the price, in Chaos Orbs, of one unit of this item.
			[[nodiscard]]
			double get_unit_price() const;
			/// @brief Get the total price, in Chaos Orbs, of all units of this item in the player's stashes.
			[[nodiscard]]
			double get_total_price() const;

		private:
			const std::string name_;
			const int64_t quantity_;
			const double unit_price_;
			const double total_price_;
	};
}  // namespace axel

#endif  // AXEL_AXEL_ITEM_H