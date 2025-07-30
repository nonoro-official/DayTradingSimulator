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

std::string Menu::BuildCompanyDropdownString() {
    std::string result;
    for (size_t i = 0; i < companies.size(); ++i) {
        result += companies[i]->GetName();
        if (i != companies.size() - 1) result += ";";
    }
    return result;
}

void Menu::Init(GameState* gameRef)
{
    game = gameRef;
    upgradeHandler.init(*game);
    game->AddTickListener([]() {
        TransactionManager::Instance().Update();
    });

    game->InitializeCompaniesAndStocks();

    dashboardScreen = new DashboardScreen(&game->GetCompanies(), &game->GetSelectedCompanyIndex());
    companiesScreen = new CompaniesScreen(&game->GetCompanies());
    portfolioScreen = new PortfolioScreen();
    upgradesScreen = new UpgradesScreen(&upgradeHandler, &player, &message);
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
    delete graphDisplay;

    for (Company* c : companies)
    {
        delete c;
    }
    companies.clear();
}
