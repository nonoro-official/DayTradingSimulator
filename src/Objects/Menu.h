//
// Created by Noah Peñaranda on 7/24/2025.
//

#pragma once
#include "Objects/Graph.h"
#include "Objects/GenerateRandomMarket.h"
#include "../Classes/GameState.h"
#include <vector>
#include <string>

#include "Upgrades/UpgradeHandler.h"

enum Screen
{
    SCREEN_DASHBOARD,
    SCREEN_PORTFOLIO,
    SCREEN_COMPANIES,
    SCREEN_UPGRADES
};

struct OwnedCompany
{
    std::string name;
    float shares;
    float value;
    float increase;
};

class Menu {
private:
    Screen currentScreen = SCREEN_DASHBOARD;

    GraphDisplay* graphDisplay = nullptr;
    GameState* game = nullptr;
    UpgradeHandler upgradeHandler;

    std::vector<OwnedCompany> portfolio = {
        {"Company A", 10, 100, -1},
        {"Company B", 5, 200, 15},
        { "Company C", 10, 10, -1}
    };
public:
    void Init(GameState* gameRef);
    void SetScreen(Screen screen);
    void Draw();
    void DrawPortfolioScreen();
    void DrawCompaniesScreen();
    void DrawUpgradesScreen();
    void Update();

    ~Menu();
};
