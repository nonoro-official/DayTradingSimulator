#include "raylib.h"
#include "raygui.h"
#include "Classes/GameState.h"
#include "Classes/PlayerData.h"
#include "Objects/Menu.h"
#include "Objects/MonthDisplay.h"

const int screenWidth = 960;
const int screenHeight = 540;

static Menu menu;

void DrawTopBar() {
    DrawRectangle(0, 0, screenWidth, 60, DARKGRAY);  // increase height for 2 rows

    float profit = PlayerData::Instance().GetTotalPortfolioValue() - PlayerData::Instance().cash;

    std::string line1 = "Cash: $" + std::to_string(PlayerData::Instance().cash) +
                        " | Portfolio Value: $" + std::to_string(PlayerData::Instance().GetTotalPortfolioValue());

    std::string line2 = std::string("Total P/L: ") + (profit >= 0 ? "+" : "") + std::to_string(profit);

    DrawText(line1.c_str(), 140, 10, 20, RAYWHITE);
    DrawText(line2.c_str(), 140, 35, 20, (profit >= 0) ? GREEN : RED);

}

void DrawSidebar() {
    DrawRectangle(0, 0, 120, screenHeight, LIGHTGRAY);

    if (GuiButton({10.0f, 60.0f, 100.0f, 40.0f}, "Portfolio"))
        menu.SetScreen(SCREEN_PORTFOLIO);

    if (GuiButton({10.0, 110.0f, 100.0f, 40.0f}, "Dashboard"))
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

    // TODO: TEMPORARY
    MonthDisplay m = MonthDisplay(28, {880, 50}, {200,100},2,BLACK, LIGHTGRAY, BLACK);

    while (!WindowShouldClose()) {
        // Global Controls
        if (IsKeyPressed(KEY_P)) GameState::Instance().PauseGame();
        if (IsKeyPressed(KEY_ONE)) GameState::Instance().SetTimeScale(1.0f);
        if (IsKeyPressed(KEY_TWO)) GameState::Instance().SetTimeScale(2.0f);
        if (IsKeyPressed(KEY_THREE)) GameState::Instance().SetTimeScale(3.0f);
        if (IsKeyPressed(KEY_FOUR)) GameState::Instance().SetTimeScale(12.0f);

        GameState::Instance().Update();
        menu.Update();

        BeginDrawing();
        ClearBackground(RAYWHITE);

        menu.Draw();
        DrawTopBar();
        DrawSidebar();

        m.Update();
        m.Draw(); // TODO: TEMPORARY

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
