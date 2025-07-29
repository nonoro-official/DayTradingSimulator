#include "raylib.h"
#include "raygui.h"
#include "Classes/GameState.h"
#include "Classes/PlayerData.h"
#include "Menu/Menu.h"
#include "Objects/MonthDisplay.h"

const int screenWidth = 960;
const int screenHeight = 540;

static Menu menu;

void DrawTopBar() {
    DrawRectangle(0, 0, screenWidth, 60, DARKGRAY);  // increase height for 2 rows

    float cash = PlayerData::Instance().cash;
    float portfolio = PlayerData::Instance().GetTotalPortfolioValue();
    float profit = portfolio - cash;

    std::ostringstream line1Stream, line2Stream;
    line1Stream << std::fixed << std::setprecision(2);
    line2Stream << std::fixed << std::setprecision(2);

    line1Stream << "Cash: $" << cash << " | Portfolio Value: $" << portfolio;
    line2Stream << "Total P/L: " << (profit >= 0 ? "+" : "") << profit;

    std::string line1 = line1Stream.str();
    std::string line2 = line2Stream.str();

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
    MonthDisplay m = MonthDisplay(20, {screenWidth - 100, 30}, {200,60},2,BLACK, LIGHTGRAY, BLACK);

    while (!WindowShouldClose()) {
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
