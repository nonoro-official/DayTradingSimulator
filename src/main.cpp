#include "raylib.h"
#include "Objects/Graph.h"
#include "Objects/GenerateRandomMarket.h"
#include "GameState.h"
#include "Upgrades/UpgradeHandler.h"
#include <iostream>

const int screenWidth = 640;
const int screenHeight = 480;

static GameState game;
int main() {
    /*
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

    return 0;*/

    InitWindow(screenWidth, screenHeight, "Day Trading Simulator");

    SetTargetFPS(60);

    GenerateRandomMarket *market = new GenerateRandomMarket(100, 1, .25);

    GraphDisplay display = GraphDisplay({screenWidth / 2, screenHeight / 2},
                                            {screenWidth, screenHeight / 1.5f}, &game);
    market->InitializeMarket();
    display.AddNodesFromVector(market->GetMarketValues());

    while (!WindowShouldClose()) {
        // pausing
        if (IsKeyPressed(KEY_P)) {
            game.PauseGame();
        }

        if (IsKeyPressed(KEY_ONE)) game.SetTimeScale(1.0f);
        if (IsKeyPressed(KEY_TWO)) game.SetTimeScale(2.0f);
        if (IsKeyPressed(KEY_THREE)) game.SetTimeScale(3.0f);
        if (IsKeyPressed(KEY_FOUR)) game.SetTimeScale(4.0f);

        if (!game.IsPaused()) {
            display.Update();
        }

        BeginDrawing();
        ClearBackground(WHITE);

        display.Draw();

        EndDrawing();
    }

    delete market;
    CloseWindow();
}

