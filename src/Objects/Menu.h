#pragma once
#include "Objects/Graph.h"
#include "Objects/GenerateRandomMarket.h"
#include "../Classes/GameState.h"
#include "../Classes/Company.h"
#include "../Classes/PlayerData.h"
#include "Upgrades/UpgradeHandler.h"
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
    MessageDisplay message;

    // Dropdown state
    int selectedCompanyIndex = 0;
    bool dropdownActive = false;
    std::vector<Company*> companies;  // Use this to store all companies
    std::vector<Stock*> stocks;

    // Search bar
    char searchText[32] = "Search...";
    bool isSearchFocused = false;

public:
    void InitializeCompanies();
    void InitializeStocks();

    void Init(GameState* gameRef);
    Screen GetCurrentScreen();
    void SetScreen(Screen screen);
    void Update();
    void Draw();
    std::string BuildCompanyDropdownString();

    void DrawDashboardScreen();
    void DrawPortfolioScreen();
    void DrawCompaniesScreen();
    void DrawUpgradesScreen();

    ~Menu();
};
