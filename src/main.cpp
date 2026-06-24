#include "raylib.h"
#include "raygui.h"
#include "Classes/GameState.h"
#include "Menu/Menu.h"
#include "Resources.h"

const int screenWidth = 960;
const int screenHeight = 540;

static Menu menu;

int main() {
    InitWindow(screenWidth, screenHeight, "Day Trading Simulator");
    
    // Set Window Icon
    Image icon = LoadImageFromMemory(".png", day_trading_logo_png, day_trading_logo_png_size);
    SetWindowIcon(icon);
    UnloadImage(icon);

    SetTargetFPS(60);

    menu.Init(&GameState::Instance());

    while (!WindowShouldClose()) {
        GameState::Instance().Update();
        menu.Update();

        BeginDrawing();
        ClearBackground(RAYWHITE);

        menu.Draw();

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
