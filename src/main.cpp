#include "GameState.h"
#include "Upgrades/UpgradeHandler.h"
#include <iostream>

int main() {
    GameState game;
    UpgradeHandler shop;
    shop.init(game);

    while (true) {
        std::cout << "\nCash: $" << game.cash
                  << " | Delay: " << game.executionDelay << "s\n";
        shop.showAvailable();

        std::cout << "Choose upgrade (index) or -1 to exit: ";
        int index;
        std::cin >> index;

        if (index == -1) break;

        shop.handlePurchase(index, game);  // <== This applies the upgrade
    }

    return 0;
}
