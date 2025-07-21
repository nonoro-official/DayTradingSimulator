#include "raylib.h"
#include "Objects/Graph.h"

const int screenWidth = 640;
const int screenHeight = 480;

int main()
{
    InitWindow(screenWidth, screenHeight, "Day Trading Simulator");
    SetTargetFPS(60);

    GraphPoint points[] = {
        { screenWidth / 2, screenHeight / 2 },
        { screenWidth / 2 - 15 * 2, screenHeight / 2 - 15 },
        { screenWidth / 2 - 30 * 2, screenHeight / 2 },
        { screenWidth / 2 - 45 * 2, screenHeight / 2 + 15 }
    };

    GraphDisplay display;

    for (GraphPoint& point : points) {
        display.AddNode(&point);
    }

    while (!WindowShouldClose()) {
        // Update
        display.Update();

        // Draw
        BeginDrawing();
        ClearBackground(WHITE);
        display.Draw();
        EndDrawing();
    }

    CloseWindow();
    return 0;
}
