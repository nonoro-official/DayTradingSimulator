//
// Created by Noah Peñaranda on 7/24/2025.
//

#include "Menu.h"
#include "raylib.h"
#include "raygui.h"
#include "Classes/Company.h"
#include "Classes/Stock.h"
#include "Classes/PlayerData.h"

bool GraphDisplay::isAnyHovering = false;

void Menu::InitializeCompanies() {
    Company* lemon = new Company("Lemon Inc", "Develops sustainable tech for modern agriculture.", 0.6f, 0.9f, new GraphDisplay({540, 300}, {840, 360}));
    lemon->market->SetNormalValues(GenerateRandomMarket::PerlinNoise, 0.3f);
    lemon->market->SetTrendValues(GenerateRandomMarket::PerlinNoise, 0.25f, {8, 20}, {-0.01f, 0.02f});
    lemon->market->SetHoldValues(GenerateRandomMarket::WhiteNoise, 0.05f, {6, 12});
    lemon->market->SetVolatileValues(GenerateRandomMarket::WhiteNoise, 0.8f, {3, 6});
    lemon->SetStoreValues(
        "A slow-growing tech firm focused on sustainable agriculture and automation.\n"
        "Stable and low-risk, it’s ideal for long-term investors looking for consistent gains.",
        52.00f
    );
    companies.push_back(lemon);

    Company* banana = new Company("Banana Corp", "Builds AI systems for automated fruit farming.", 1.25f, 1.4f, new GraphDisplay({540, 300}, {840, 360}));
    banana->market->SetNormalValues(GenerateRandomMarket::WhiteNoise, 0.6f);
    banana->market->SetTrendValues(GenerateRandomMarket::WhiteNoise, 0.5f, {3, 8}, {-0.04f, 0.06f});
    banana->market->SetHoldValues(GenerateRandomMarket::WhiteNoise, 0.15f, {2, 5});
    banana->market->SetVolatileValues(GenerateRandomMarket::WhiteNoise, 1.6f, {4, 10});
    banana->SetStoreValues(
        "A volatile AI startup blending biotech with fruit farming.\n"
        "High-risk, high-reward — perfect for traders chasing sharp spikes and quick flips.",
        38.00f
    );
    companies.push_back(banana);

    Company* mango = new Company("Mango Ltd", "Manages cargo and logistics across tropical regions.", 0.9f, 1.1f, new GraphDisplay({540, 300}, {840, 360}));
    mango->market->SetNormalValues(GenerateRandomMarket::PerlinNoise, 0.4f);
    mango->market->SetTrendValues(GenerateRandomMarket::WhiteNoise, 0.3f, {6, 14}, {-0.015f, 0.035f});
    mango->market->SetHoldValues(GenerateRandomMarket::WhiteNoise, 0.1f, {4, 10});
    mango->market->SetVolatileValues(GenerateRandomMarket::PerlinNoise, 1.2f, {5, 12});
    mango->SetStoreValues(
        "A dependable logistics company serving tropical trade routes.\n"
        "Moderately stable with occasional bursts, it's best bought before seasonal demand ramps up.",
        46.00f
    );
    companies.push_back(mango);
}

void Menu::InitializeStocks() {
    for (Company * company: companies) {
        Stock* stock = new Stock(*company, .5f);
        stocks.push_back(stock);
    }
}

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

    // Initialize Companies
    InitializeCompanies();

    // Initialize Stocks
    InitializeStocks();

    // Initialize Screens
    dashboardScreen = new DashboardScreen(&companies, &selectedCompanyIndex);
    companiesScreen = new CompaniesScreen(&companies);
    portfolioScreen = new PortfolioScreen();
    upgradesScreen = new UpgradesScreen(&upgradeHandler, &player, &message);
}

//
// void Menu::Update() {
//     GraphDisplay::isAnyHovering = false;
//
//     if (currentScreen == SCREEN_DASHBOARD) {
//         for (int i = 0; i < companies.size(); i++) {
//             if (i == selectedCompanyIndex) companies[i]->display->active = true;
//             else companies[i]->display->active = false;
//             companies[i]->display->Update();
//         }
//     }
// }

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
