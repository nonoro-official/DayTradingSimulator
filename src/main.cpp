#include "raylib.h"
#include "Objects/Graph.h"
#include "Objects/GenerateRandomMarket.h"
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

        GenerateRandomMarket *market = new GenerateRandomMarket(100, 1, .25);
        market->InitializeMarket();

        GraphDisplay *display = new GraphDisplay({screenWidth / 2, screenHeight / 2},
                                                {screenWidth, screenHeight / 1.5f});

        display->AddNodesFromVector(market->GetMarketValues());
        if (index == -1) break;

        shop.handlePurchase(index, game);  // <== This applies the upgrade
    }

    return 0;
}
