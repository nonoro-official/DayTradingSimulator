//
// Created by Noah Peñaranda on 7/24/2025.
//

#include "Menu.h"
#include "raylib.h"
#include "raygui.h"
#include "Resources.h"
#include "Classes/Company.h"
#include "Classes/Stock.h"
#include "Classes/PlayerData.h"
#include "Classes/Transactions/TransactionManager.h"

bool GraphDisplay::isAnyHovering = false;

void Menu::Init(GameState* gameRef)
{
    game = gameRef;
    game->SetPause(true); // Start paused for intro/tutorial
    menuFont = LoadFontFromMemory(".ttf", VT323_Regular_ttf, VT323_Regular_ttf_size, 30, NULL, 0);
    upgradeHandler.init(*game);
    game->AddTickListener([]() {
        TransactionManager::Instance().Update();
    });
    game->AddTickListener([this]() {
        upgradeHandler.progressWeek(PlayerData::Instance(), popup);
    });

    game->InitializeCompaniesAndStocks();

    dashboardScreen = new DashboardScreen(&game->GetCompanies(), &game->GetSelectedCompanyIndex(), &popup);
    companiesScreen = new CompaniesScreen(&game->GetCompanies(), &popup);
    portfolioScreen = new PortfolioScreen(&popup);
    upgradesScreen = new UpgradesScreen(&upgradeHandler, &PlayerData::Instance(), &popup);
    historyScreen = new HistoryScreen();

    monthDisplay = new MonthDisplay(20, {(float)GetScreenWidth() - 100, 30}, {200,60}, 2, BLACK, LIGHTGRAY, BLACK);
}

void Menu::Update() {
    if (currentScreen == SCREEN_INTRO) return;

    if (isTutorialActive) {
        // Only update tutorial-related things if needed
        return;
    }

    GraphDisplay::isAnyHovering = false;

    switch (currentScreen) {
    case SCREEN_DASHBOARD:
        if (dashboardScreen) dashboardScreen->Update();
        break;
    case SCREEN_PORTFOLIO:
        // if (portfolioScreen) portfolioScreen->Update();
        break;
    case SCREEN_COMPANIES:
        // if (companiesScreen) companiesScreen->Update();
        break;
    case SCREEN_UPGRADES:
        // if (upgradesScreen) upgradesScreen->Update();
        break;
    }

    if (monthDisplay) monthDisplay->Update();
}

Screen Menu::GetCurrentScreen()
{
    return currentScreen;
}

void Menu::SetScreen(Screen screen)
{
    currentScreen = screen;
}

void Menu::Draw() {
    if (currentScreen == SCREEN_INTRO) {
        DrawIntroScreen();
        return;
    }

    switch (currentScreen) {
    case SCREEN_DASHBOARD:
        if (dashboardScreen) dashboardScreen->Draw();
        break;
    case SCREEN_PORTFOLIO:
        if (portfolioScreen) portfolioScreen->Draw();
        break;
    case SCREEN_COMPANIES:
        if (companiesScreen) companiesScreen->Draw();
        break;
    case SCREEN_UPGRADES:
        if (upgradesScreen) upgradesScreen->Draw();
        break;
    case SCREEN_HISTORY:
        if (historyScreen) historyScreen->Draw();
        break;
    }

    DrawTopBar();
    DrawSidebar();

    if (monthDisplay) monthDisplay->Draw();

    if (isTutorialActive) {
        DrawTutorialOverlay();
    }
}

void Menu::DrawIntroScreen() {
    int sw = GetScreenWidth();
    int sh = GetScreenHeight();

    const char* title = "THE way to get rich";
    const char* subtext = "(do not replicate at home)";
    const char* description =
        "Welcome to the high-stakes world of fruit-tech trading.\n"
        "Start with $150 and turn it into a fortune.\n"
        "Trade stocks, unlock powerful upgrades, and master the market.";

    float titleSize = 60.0f;
    float subtextSize = 25.0f;
    float descSize = 22.0f;

    Vector2 titleDim = MeasureTextEx(menuFont, title, titleSize, 2);
    Vector2 subtextDim = MeasureTextEx(menuFont, subtext, subtextSize, 2);
    Vector2 descDim = MeasureTextEx(menuFont, description, descSize, 2);

    DrawTextEx(menuFont, title, {(float)(sw - titleDim.x) / 2.0f, (float)sh / 2.0f - 120.0f}, titleSize, 2.0f, BLACK);
    DrawTextEx(menuFont, subtext, {(float)(sw - subtextDim.x) / 2.0f, (float)sh / 2.0f - 50.0f}, subtextSize, 2.0f, DARKGRAY);
    DrawTextEx(menuFont, description, {(float)(sw - descDim.x) / 2.0f, (float)sh / 2.0f + 10.0f}, descSize, 2.0f, BLACK);

    if (GuiButton({ (float)sw / 2 - 100, (float)sh / 2 + 110, 200, 50 }, "Enter Market")) {
        currentScreen = SCREEN_DASHBOARD;
        isTutorialActive = true;
        tutorialStep = 0;
    }
}

void Menu::DrawTutorialOverlay() {
    int sw = GetScreenWidth();
    int sh = GetScreenHeight();

    // Darken background
    DrawRectangle(0, 0, sw, sh, Fade(BLACK, 0.4f));

    Rectangle tutorialBox = { (float)sw / 2 - 250, (float)sh / 2 - 120, 500, 240 };
    DrawRectangleRec(tutorialBox, RAYWHITE);
    DrawRectangleLinesEx(tutorialBox, 2, BLACK);

    const char* tutorialTitle = "Tutorial";
    const char* tutorialText = "";
    Rectangle highlight = { 0, 0, 0, 0 };
    Rectangle btnHighlight = { 0, 0, 0, 0 };

    switch (tutorialStep) {
    case 0:
        tutorialTitle = "The Sidebar";
        tutorialText = "This is the Sidebar. Use it to navigate\nbetween your Portfolio, the Dashboard,\nand more screens.";
        highlight = { 0, 0, 120, (float)sh };
        break;
    case 1:
        tutorialTitle = "The Top Bar";
        tutorialText = "The Top Bar shows your Cash and your\ntotal Portfolio Value. Watch your\nProfit/Loss here!";
        highlight = { 120, 0, (float)sw - 120, 60 };
        break;
    case 2:
        tutorialTitle = "Time Controls";
        tutorialText = "Here you can control time. Speed up the\nweeks or pause to rethink your strategy.\nKeybinds: [P] Pause, [1]-[3] Speed";
        highlight = { (float)sw - 200, 0, 200, 60 };
        break;
    case 3:
        tutorialTitle = "Trading";
        tutorialText = "In the Dashboard, select a company and\nuse BUY/SELL to trade.\nRemember: Buy low, sell high!";
        highlight = { 130, 70, (float)sw - 140, (float)sh - 80 };
        btnHighlight = { 10, 110, 100, 40 };
        break;
    case 4:
        tutorialTitle = "Portfolio";
        tutorialText = "The Portfolio shows all the stocks you\ncurrently own. Check your holdings\nand their current value here.";
        highlight = { 130, 70, (float)sw - 140, (float)sh - 80 };
        btnHighlight = { 10, 20, 100, 80 };
        break;
    case 5:
        tutorialTitle = "Companies";
        tutorialText = "The Companies screen gives you details\nabout every firm in the market.\nKnow your assets!";
        highlight = { 130, 70, (float)sw - 140, (float)sh - 80 };
        btnHighlight = { 10, 160, 100, 40 };
        break;
    case 6:
        tutorialTitle = "Upgrades";
        tutorialText = "Spend your profits on Upgrades to get\nmarket advantages like faster execution\nor prediction hints.";
        highlight = { 130, 70, (float)sw - 140, (float)sh - 80 };
        btnHighlight = { 10, 210, 100, 40 };
        break;
    case 7:
        tutorialTitle = "History";
        tutorialText = "The History log keeps track of all your\npast trades. Analyze your performance\nover time.";
        highlight = { 130, 70, (float)sw - 140, (float)sh - 80 };
        btnHighlight = { 10, 260, 100, 40 };
        break;
    case 8:
        tutorialTitle = "Final Advice";
        tutorialText = "Good luck! You're ready to become a\nmarket legend. (Or lose it all,\nbut hey, it's a simulator!)";
        break;
    }

    DrawTextEx(menuFont, tutorialTitle, {tutorialBox.x + 20.0f, tutorialBox.y + 20.0f}, 30.0f, 2.0f, MAROON);
    DrawTextEx(menuFont, tutorialText, {tutorialBox.x + 20.0f, tutorialBox.y + 70.0f}, 24.0f, 2.0f, BLACK);

    if (highlight.width > 0) {
        DrawRectangleLinesEx(highlight, 3, RED);
    }
    if (btnHighlight.width > 0) {
        DrawRectangleLinesEx(btnHighlight, 3, RED);
    }

    if (GuiButton({ tutorialBox.x + tutorialBox.width - 130, tutorialBox.y + tutorialBox.height - 60, 110, 45 }, tutorialStep < 8 ? "Next" : "Finish")) {
        tutorialStep++;
        if (tutorialStep == 4) currentScreen = SCREEN_PORTFOLIO;
        else if (tutorialStep == 5) currentScreen = SCREEN_COMPANIES;
        else if (tutorialStep == 6) currentScreen = SCREEN_UPGRADES;
        else if (tutorialStep == 7) currentScreen = SCREEN_HISTORY;
        else if (tutorialStep == 8) currentScreen = SCREEN_DASHBOARD;

        if (tutorialStep > 8) {
            isTutorialActive = false;
            game->SetPause(false); // Unpause when tutorial is done
        }
    }
}

void Menu::DrawTopBar() {
    int sw = GetScreenWidth();
    DrawRectangle(0, 0, sw, 60, DARKGRAY);

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

void Menu::DrawSidebar() {
    int sh = GetScreenHeight();
    DrawRectangle(0, 0, 120, sh, LIGHTGRAY);

    const char* labels[] = { "Portfolio", "Dashboard", "Companies", "Upgrades", "History" };
    Screen screens[] = { SCREEN_PORTFOLIO, SCREEN_DASHBOARD, SCREEN_COMPANIES, SCREEN_UPGRADES, SCREEN_HISTORY };
    const int buttonCount = 5;

    int baseNormal = GuiGetStyle(BUTTON, BASE_COLOR_NORMAL);
    int borderNormal = GuiGetStyle(BUTTON, BORDER_COLOR_NORMAL);

    float y = 20.0f; // start position

    for (int i = 0; i < buttonCount; ++i) {
        float btnHeight = (i == 0) ? 80.0f : 40.0f;  // Make Portfolio taller
        Rectangle btn = {10.0f, y, 100.0f, btnHeight};

        bool isActive = currentScreen == screens[i];

        if (isActive) {
            GuiSetStyle(BUTTON, BASE_COLOR_NORMAL, GuiGetStyle(BUTTON, BASE_COLOR_FOCUSED));
            GuiSetStyle(BUTTON, BORDER_COLOR_NORMAL, GuiGetStyle(BUTTON, BORDER_COLOR_FOCUSED));
        } else {
            GuiSetStyle(BUTTON, BASE_COLOR_NORMAL, baseNormal);
            GuiSetStyle(BUTTON, BORDER_COLOR_NORMAL, borderNormal);
        }

        if (isTutorialActive) GuiSetState(STATE_DISABLED);
        if (GuiButton(btn, labels[i])) {
            SetScreen(screens[i]);
        }
        if (isTutorialActive) GuiSetState(STATE_NORMAL);

        y += btnHeight + 10.0f; // vertical spacing
    }

    // Restore styles
    GuiSetStyle(BUTTON, BASE_COLOR_NORMAL, baseNormal);
    GuiSetStyle(BUTTON, BORDER_COLOR_NORMAL, borderNormal);
}

Menu::~Menu()
{
    UnloadFont(menuFont);
    delete dashboardScreen;
    delete companiesScreen;
    delete portfolioScreen;
    delete upgradesScreen;
    delete historyScreen;
    delete monthDisplay;
}
