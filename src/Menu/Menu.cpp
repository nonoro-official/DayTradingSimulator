//
// Created by Noah Peñaranda on 7/24/2025.
//

#include "Menu.h"
#include "raylib.h"
#include "raygui.h"
#include "Classes/Company.h"
#include "Classes/Stock.h"
#include "Classes/PlayerData.h"
#include "Classes/Transactions/TransactionManager.h"

bool GraphDisplay::isAnyHovering = false;

void Menu::Init(GameState* gameRef)
{
    game = gameRef;
    upgradeHandler.init(*game);
    game->AddTickListener([]() {
        TransactionManager::Instance().Update();
    });
    game->AddTickListener([this]() {
        upgradeHandler.progressWeek(player, popup);
    });

    game->InitializeCompaniesAndStocks();

    dashboardScreen = new DashboardScreen(&game->GetCompanies(), &game->GetSelectedCompanyIndex(), &popup);
    companiesScreen = new CompaniesScreen(&game->GetCompanies(), &popup);
    portfolioScreen = new PortfolioScreen(&popup);
    upgradesScreen = new UpgradesScreen(&upgradeHandler, &player, &popup);
    historyScreen = new HistoryScreen();
}

void Menu::Update() {
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
}

Menu::~Menu()
{
    delete dashboardScreen;
    delete companiesScreen;
    delete portfolioScreen;
    delete upgradesScreen;
    delete historyScreen;
}
