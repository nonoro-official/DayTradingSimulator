#pragma once
#include "Objects/Graph.h"
#include "Objects/GenerateRandomMarket.h"
#include "../Classes/GameState.h"
#include "../Classes/Company.h"
#include "../Classes/PlayerData.h"
#include "Upgrades/UpgradeHandler.h"
#include "Objects/PopUpWindow.h"
#include "Screens/CompaniesScreen.h"
#include "Screens/DashboardScreen.h"
#include "Screens/PortfolioScreen.h"
#include "Screens/UpgradesScreen.h"
#include <vector>
#include <cstring>
#include <sstream>
#include <iomanip>

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
    PlayerData player;
    PopUpWindow message;

    // Dropdown state
    int selectedCompanyIndex = 0;
    bool dropdownActive = false;
    std::vector<Company*> companies;  // Use this to store all companies

    // Search bar
    char searchText[32] = "Search...";
    bool isSearchFocused = false;

    // Screens
    DashboardScreen* dashboardScreen = nullptr;
    PortfolioScreen* portfolioScreen = nullptr;
    CompaniesScreen* companiesScreen = nullptr;
    UpgradesScreen* upgradesScreen = nullptr;

public:
    void InitializeCompanies();
    void InitializeStocks();
    void Init(GameState* gameRef);

    Screen GetCurrentScreen();
    void SetScreen(Screen screen);

    void Update();
    void Draw();

    std::string BuildCompanyDropdownString();

    ~Menu();
};
