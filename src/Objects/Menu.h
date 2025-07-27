#pragma once
#include "Objects/Graph.h"
#include "Objects/GenerateRandomMarket.h"
#include "../Classes/GameState.h"
#include "../Classes/Company.h"
#include "../Classes/PlayerData.h"
#include "Upgrades/UpgradeHandler.h"
#include <vector>
#include <string>

enum Screen
{
    SCREEN_DASHBOARD,
    SCREEN_PORTFOLIO,
    SCREEN_COMPANIES,
    SCREEN_UPGRADES
};

class Menu {
private:
    Screen currentScreen = SCREEN_DASHBOARD;

    GraphDisplay* graphDisplay = nullptr;
    GameState* game = nullptr;
    UpgradeHandler upgradeHandler;

    // Dropdown state
    int selectedCompanyIndex = 0;
    bool dropdownActive = false;
    std::vector<Company*> companies;  // Use this to store all companies

public:
    void Init(GameState* gameRef);
    void SetScreen(Screen screen);
    void Update();
    void Draw();

    void DrawDashboardScreen();
    void DrawPortfolioScreen();
    void DrawCompaniesScreen();
    void DrawUpgradesScreen();

    ~Menu();
};
