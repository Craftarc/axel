#ifndef AXEL_CONFIG_POE_NINJA_CONFIG_H
#define AXEL_CONFIG_POE_NINJA_CONFIG_H

#include <string>

/**
 * Contains definitions for all relevant parameters in poe.ninja endpoints
 * paths: currently available currency/item types
 * leagues: currently available leagues
 *
 * Last updated: Crucible 7/6/2023
 */
namespace config {
    namespace poe_ninja {
        const std::string host = "poe.ninja";
        namespace paths {
            const std::string currency = "/api/data/currencyoverview?type=Currency";
            const std::string fragment = "/api/data/currencyoverview?type=Fragment";
            const std::string divination_card = "/api/data/itemoverview?type=DivinationCard";
            const std::string artifact = "/api/data/itemoverview?type=Artifact";
            const std::string oil = "/api/data/itemoverview?type=Oil";
            const std::string incubator = "/api/data/itemoverview?type=Incubator";
            const std::string unique_weapon = "/api/data/itemoverview?type=UniqueWeapon";
            const std::string unique_armour = "/api/data/itemoverview?type=UniqueArmour";
            const std::string unique_accessory = "/api/data/itemoverview?type=UniqueAccessory";
            const std::string unique_flask = "/api/data/itemoverview?type=UniqueFlask";
            const std::string unique_jewel = "/api/data/itemoverview?type=UniqueJewel";
            const std::string skill_gem = "/api/data/itemoverview?type=SkillGem";
            const std::string cluster_jewel = "/api/data/itemoverview?type=ClusterJewel";
            const std::string map = "/api/data/itemoverview?type=Map";
            const std::string blighted_map = "/api/data/itemoverview?type=BlightedMap";
            const std::string blight_ravaged_map = "/api/data/itemoverview?type=BlightRavagedMap";
            const std::string scourged_map = "/api/data/itemoverview?type=ScourgedMap";
            const std::string unique_map = "/api/data/itemoverview?type=UniqueMap";
            const std::string delirium_orb = "/api/data/itemoverview?type=DeliriumOrb";
            const std::string invitation = "/api/data/itemoverview?type=Invitation";
            const std::string scarab = "/api/data/itemoverview?type=Scarab";
            const std::string base_type = "/api/data/itemoverview?type=BaseType";
            const std::string fossil = "/api/data/itemoverview?type=Fossil";
            const std::string resonator = "/api/data/itemoverview?type=Resonator";
            const std::string helmet_enchant = "/api/data/itemoverview?type=HelmetEnchant";
            const std::string beast = "/api/data/itemoverview?type=Beast";
            const std::string essence = "/api/data/itemoverview?type=Essence";
            const std::string vial = "/api/data/itemoverview?type=Vial";
        } // paths
        
        namespace leagues {
            const std::string crucible = "Crucible";
            const std::string sanctum = "Sanctum";
        } // leagues
    } // poe_ninja
} // config
#endif //AXEL_CONFIG_POE_NINJA_CONFIG_H
