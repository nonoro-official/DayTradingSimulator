//
// Created by Noah Peñaranda on 7/24/2025.
//

#include "Menu.h"
#include "raylib.h"
#include "raygui.h"
#include "Classes/Company.h"

void Menu::Init(GameState* gameRef) {
    game = gameRef;
    upgradeHandler.init(*game);
    graphDisplay = new GraphDisplay(
    {120 + (840 / 2.0f), 40 + (500 / 2.0f)}, // position (centered in main area)
    {840, 460});                             // size (screen - topbar/sidebar)

    // Test company
    new Company("testName", "testDesc", 50.0f, 1, 1.0f, graphDisplay);
}

void Menu::Update() {
    if (currentScreen == SCREEN_DASHBOARD && !game->IsPaused()) {
        graphDisplay->Update();
    }
}

void Menu::SetScreen(Screen screen)
{
    currentScreen = screen;
}

void Menu::Draw()
{
    if (currentScreen == SCREEN_DASHBOARD) graphDisplay->Draw();
    else if (currentScreen == SCREEN_PORTFOLIO) DrawPortfolioScreen();
    else if (currentScreen == SCREEN_COMPANIES) DrawCompaniesScreen();
    else if (currentScreen == SCREEN_UPGRADES) DrawUpgradesScreen();
}

void Menu::DrawPortfolioScreen()
{
    DrawText("PORTFOLIO", 140, 70, 30, DARKGRAY);

    GuiTextBox((Rectangle){GetScreenWidth() - 210.0f, 70.0f, 180.0f, 30.0f}, (char *)"Search...", 32, false);

    float boxStartY = 110.0f;
    float boxHeight = 80.0f;
    float boxWidth = GetScreenWidth() - 160.0f;

    for (size_t i = 0; i < portfolio.size(); ++i) {
        float y = boxStartY + i * (boxHeight + 10.0f);
        Rectangle box = {140.0f, y, boxWidth, boxHeight};

        DrawRectangleRec(box, LIGHTGRAY);
        DrawRectangleLinesEx(box, 1, GRAY);

        const OwnedCompany& comp = portfolio[i];

        std::string info = comp.name +
            " | shares: " + std::to_string(comp.shares) +
            ", value: " + std::to_string(comp.value) +
            ", increase: " + (comp.increase >= 0 ? "+" : "") + std::to_string(comp.increase);

        DrawText(info.c_str(), box.x + 10.0f, box.y + 10.0f, 18, BLACK);

        if (GuiButton({box.x + box.width - 160.0f, box.y + 10.0f, 60.0f, 30.0f}, "Buy")) {
            // TODO: hook up later
        }
        if (GuiButton({box.x + box.width - 80.0f, box.y + 10.0f, 60.0f, 30.0f}, "Sell")) {
            // TODO: hook up later
        }
    }
}

void Menu::DrawCompaniesScreen()
{
    DrawText("COMPANIES", 140, 70, 30, DARKGRAY);
}

void Menu::DrawUpgradesScreen()
{
    DrawText("UPGRADES", 140, 70, 30, DARKGRAY);
}

Menu::~Menu()
{
    delete graphDisplay;
}
