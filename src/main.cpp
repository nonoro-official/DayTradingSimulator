#include "raylib.h"
#include "Objects/Graph.h"
#include "Objects/GenerateRandomMarket.h"

const int screenWidth = 640;
const int screenHeight = 480;

float frameTime = 0;

int main()
{
    InitWindow(screenWidth, screenHeight, "Day Trading Simulator");
    SetTargetFPS(60);

    GenerateRandomMarket *market = new GenerateRandomMarket(100, 1, .25);
    market->InitializeMarket();

    GraphDisplay *display = new GraphDisplay({screenWidth / 2, screenHeight / 2},
                                             {screenWidth, screenHeight / 1.5f});

    display->AddNodesFromVector(market->GetMarketValues());

    while (!WindowShouldClose()) {
        // Update
        display->Update();

        // Draw
        BeginDrawing();
        ClearBackground(WHITE);
        display->Draw();
        EndDrawing();
    }

    CloseWindow();
    return 0;
}
