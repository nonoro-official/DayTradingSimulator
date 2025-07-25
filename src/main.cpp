#include "raylib.h"
#include "raygui.h"
#include "GameState.h"
#include "Objects/Menu.h"

const int screenWidth = 960;
const int screenHeight = 540;

static Menu menu;

void DrawTopBar() {
    DrawRectangle(0, 0, screenWidth, 60, DARKGRAY);  // increase height for 2 rows

    float latestPrice = 100.0f;  // or get it from your GraphDisplay
    float profit = GameState::Instance().GetTotalProfitLoss(latestPrice);

    std::string line1 = "Cash: $" + std::to_string(GameState::Instance().cash) +
                        " | Shares: " + std::to_string(GameState::Instance().sharesHeld) +
                        " | Average Buy Price: " + std::to_string(GameState::Instance().avgBuyPrice);

    std::string line2 = "Current Price: " + std::to_string(latestPrice) +
                        " | P/L: " + (profit >= 0 ? "+" : "") + std::to_string(profit);

    DrawText(line1.c_str(), 140, 10, 18, RAYWHITE);
    DrawText(line2.c_str(), 140, 35, 18, (profit >= 0) ? GREEN : RED);

    // Add week & month later on top right
    // Optional: Add Play / Pause / Fast-forward icons later
}

void DrawSidebar() {
    DrawRectangle(0, 0, 120, screenHeight, LIGHTGRAY);

    if (GuiButton({10.0f, 60.0f, 100.0f, 40.0f}, "Portfolio"))
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

    menu.Init(&GameState::Instance());

    while (!WindowShouldClose()) {
        // Global Controls
        if (IsKeyPressed(KEY_P)) GameState::Instance().PauseGame();
        if (IsKeyPressed(KEY_ONE)) GameState::Instance().SetTimeScale(1.0f);
        if (IsKeyPressed(KEY_TWO)) GameState::Instance().SetTimeScale(2.0f);
        if (IsKeyPressed(KEY_THREE)) GameState::Instance().SetTimeScale(3.0f);
        if (IsKeyPressed(KEY_FOUR)) GameState::Instance().SetTimeScale(4.0f);

        GameState::Instance().Update();
        menu.Update();

        BeginDrawing();
        ClearBackground(RAYWHITE);

        menu.Draw();
        DrawTopBar();
        DrawSidebar();

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
