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

    const char* labels[] = { "Portfolio", "Dashboard", "Companies", "Upgrades" };
    Screen screens[] = { SCREEN_PORTFOLIO, SCREEN_DASHBOARD, SCREEN_COMPANIES, SCREEN_UPGRADES };
    const int buttonCount = 4;

    int baseNormal = GuiGetStyle(BUTTON, BASE_COLOR_NORMAL);
    int borderNormal = GuiGetStyle(BUTTON, BORDER_COLOR_NORMAL);

    float y = 20.0f; // start position

    for (int i = 0; i < buttonCount; ++i) {
        float btnHeight = (i == 0) ? 80.0f : 40.0f;  // Make Portfolio taller
        Rectangle btn = {10.0f, y, 100.0f, btnHeight};

        bool isActive = menu.GetCurrentScreen() == screens[i];

        if (isActive) {
            GuiSetStyle(BUTTON, BASE_COLOR_NORMAL, GuiGetStyle(BUTTON, BASE_COLOR_FOCUSED));
            GuiSetStyle(BUTTON, BORDER_COLOR_NORMAL, GuiGetStyle(BUTTON, BORDER_COLOR_FOCUSED));
        } else {
            GuiSetStyle(BUTTON, BASE_COLOR_NORMAL, baseNormal);
            GuiSetStyle(BUTTON, BORDER_COLOR_NORMAL, borderNormal);
        }

        if (GuiButton(btn, labels[i])) {
            menu.SetScreen(screens[i]);
        }

        y += btnHeight + 10.0f; // vertical spacing
    }

    // Restore styles
    GuiSetStyle(BUTTON, BASE_COLOR_NORMAL, baseNormal);
    GuiSetStyle(BUTTON, BORDER_COLOR_NORMAL, borderNormal);
}

int main() {
    InitWindow(screenWidth, screenHeight, "Day Trading Simulator");
    SetTargetFPS(60);

    menu.Init(&GameState::Instance());

    // TODO: TEMPORARY
    MonthDisplay m = MonthDisplay(20, {880, 30}, {200,60},2,BLACK, LIGHTGRAY, BLACK);

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
