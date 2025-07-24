#include "raylib.h"
#include "raygui.h"
#include "GameState.h"
#include "Objects/Menu.h"

const int screenWidth = 960;
const int screenHeight = 540;

static GameState game;
static Menu menu;

void DrawTopBar() {
    DrawRectangle(0, 0, screenWidth, 40, DARKGRAY);

    std::string status = "Cash $" + std::to_string(game.cash) +
                         " | Shares: " + std::to_string(game.sharesHeld) ;// +
                         // " | Day: " + std::to_string(game.dayCount
                         //    );
    DrawText(status.c_str(), 140, 10, 18, RAYWHITE);

    // Optional: Add Play / Pause / Fast-forward icons later
}

void DrawSidebar() {
    DrawRectangle(0, 40, 120, screenHeight - 40, LIGHTGRAY);

    if (GuiButton({10.0f, 60.0f, 100.0f, 40.0f}, "Profile"))
        menu.SetScreen(SCREEN_PORTFOLIO);

    if (GuiButton({10.0f, 110.0f, 100.0f, 40.0f}, "Dashboard"))
        menu.SetScreen(SCREEN_DASHBOARD);

    if (GuiButton({10.0f, 160.0f, 100.0f, 40.0f}, "Companies"))
        menu.SetScreen(SCREEN_COMPANIES);

    if (GuiButton({10.0f, 210.0f, 100.0f, 40.0f}, "Upgrades"))
        menu.SetScreen(SCREEN_UPGRADES);
}

int main() {
    InitWindow(screenWidth, screenHeight, "Day Trading Simulator");
    SetTargetFPS(60);

    menu.Init(&game);

    while (!WindowShouldClose()) {
        // Global Controls
        if (IsKeyPressed(KEY_P)) game.PauseGame();
        if (IsKeyPressed(KEY_ONE)) game.SetTimeScale(1.0f);
        if (IsKeyPressed(KEY_TWO)) game.SetTimeScale(2.0f);
        if (IsKeyPressed(KEY_THREE)) game.SetTimeScale(3.0f);
        if (IsKeyPressed(KEY_FOUR)) game.SetTimeScale(4.0f);

        menu.Update();

        BeginDrawing();
        ClearBackground(RAYWHITE);

        DrawTopBar();
        DrawSidebar();
        menu.Draw();

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
