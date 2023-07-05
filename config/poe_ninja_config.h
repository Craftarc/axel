#ifndef AXEL_POE_NINJA_CONFIG_H
#define AXEL_POE_NINJA_CONFIG_H

#include <string>

namespace config {
    namespace poe_ninja {
        const std::string host = "poe.ninja";
        const std::string currency_path = "/api/data/currencyoverview?league=Crucible&type=Currency";
        const std::string fragment_path = "/api/data/currencyoverview?league=Crucible&type=Fragment";
        const std::string oil_path = "/api/data/currencyoverview?league=Crucible&type=Oil";
        const std::string incubator_path = "/api/data/currencyoverview?league=Crucible&type=Incubator";
        const std::string scarab_path = "/api/data/currencyoverview?league=Crucible&type=Scarab";
        const std::string fossil_path = "/api/data/currencyoverview?league=Crucible&type=Fossil";
        const std::string resonator_path = "/api/data/currencyoverview?league=Crucible&type=Resonator";
        const std::string essence_path = "/api/data/currencyoverview?league=Crucible&type=Essence";
        const std::string divination_card_path = "/api/data/currencyoverview?league=Crucible&type=DivinationCard";
        const std::string prophecy_path = "/api/data/currencyoverview?league=Crucible&type=Prophecy";
        const std::string skill_gem_path = "/api/data/currencyoverview?league=Crucible&type=SkillGem";
        const std::string base_type_path = "/api/data/currencyoverview?league=Crucible&type=BaseType";
        const std::string helmet_enchant_path = "/api/data/currencyoverview?league=Crucible&type=HelmetEnchant";
        const std::string unique_map_path = "/api/data/currencyoverview?league=Crucible&type=UniqueMap";
        const std::string map_path = "/api/data/currencyoverview?league=Crucible&type=Map";
        const std::string unique_jewel_path = "/api/data/currencyoverview?league=Crucible&type=UniqueJewel";
        const std::string unique_accessory = "/api/data/currencyoverview?league=Crucible&type=UniqueAccessory";
        const std::string beast_path = "/api/data/currencyoverview?league=Crucible&type=Beast";
    }
}
#endif //AXEL_POE_NINJA_CONFIG_H
