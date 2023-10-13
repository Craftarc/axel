#include <iostream>

#include "axel/Exception.h"
#include "axel/PlayerItems.h"

int main() {
    axel::PlayerItems player_items{"52009b7e7e568b1e392870014e40dbd92db501aa"};
    player_items.get_update("crucible");

}
