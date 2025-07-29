//
// Created by Noah Peñaranda on 7/24/2025.
//

#include "Menu.h"
#include "raylib.h"
#include "raygui.h"
#include "Classes/Company.h"
#include "Classes/Stock.h"
#include "Classes/PlayerData.h"
#include "Objects/Layout.h"

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

void Menu::Init(GameState* gameRef) {
    game = gameRef;
    upgradeHandler.init(*game);

    // Initialize Companies
    InitializeCompanies();

    // Initialize Stocks
    InitializeStocks();

    // Initialize Screens
    dashboardScreen = new DashboardScreen(&companies, &selectedCompanyIndex);
    companiesScreen = new CompaniesScreen(&companies);
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
        // if (portfolioScreen) portfolioScreen->Draw();
        break;
    case SCREEN_COMPANIES:
        if (companiesScreen) companiesScreen->Draw();
        break;
    case SCREEN_UPGRADES:
        // if (upgradesScreen) upgradesScreen->Draw();
        break;
    }
}

void Menu::DrawPortfolioScreen()
{
    DrawText("PORTFOLIO", 140, 70, 30, DARKGRAY);

    Rectangle searchBox = { GetScreenWidth() - 180.0f, 60.0f + 10.0f, 170.0f, 30.0f };

    // Click detection
    if (CheckCollisionPointRec(GetMousePosition(), searchBox)) {
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            isSearchFocused = true;

            // Clear placeholder when first clicked
            if (strcmp(searchText, "Search...") == 0) {
                searchText[0] = '\0'; // Clear text
            }
        }
    } else if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        isSearchFocused = false;
    }

    // Show placeholder if not focused and empty
    if (!isSearchFocused && strlen(searchText) == 0) {
        strcpy(searchText, "Search...");
    }

    // Optional styles
    GuiSetStyle(TEXTBOX, BASE_COLOR_NORMAL, ColorToInt(WHITE));
    GuiSetStyle(TEXTBOX, BORDER_COLOR_NORMAL, ColorToInt(GRAY));
    GuiSetStyle(TEXTBOX, TEXT_COLOR_NORMAL, ColorToInt(BLACK));

    // Render the textbox (editable if focused)
    GuiTextBox(searchBox, searchText, 32, isSearchFocused);

    float boxStartY = 110.0f;
    float boxHeight = 80.0f;
    float boxWidth = GetScreenWidth() - 160.0f;

    std::vector<Stock>& stocks = PlayerData::Instance().GetStocks();

    for (size_t i = 0; i < stocks.size(); ++i) {
        Stock& stock = stocks[i];
        Company* company = stock.company;

        float shares = stock.shares;

        if (shares < stock.minimumShares) { continue; }

        float value = stock.GetShareValue();
        float increase = company->CalculateIncreaseFromWeeksAgo(12);

        float y = boxStartY + i * (boxHeight + 10.0f);
        Rectangle box = {140.0f, y, boxWidth, boxHeight};

        DrawRectangleRec(box, LIGHTGRAY);
        DrawRectangleLinesEx(box, 1, GRAY);

        std::string info = company->companyName +
            " | Shares: " + std::to_string((int)shares) +
            " | Value: $" + std::to_string((int)value) +
            " | Increase: " + (increase >= 0 ? "+" : "") + std::to_string((int)increase) + "%";

        DrawText(info.c_str(), box.x + 10.0f, box.y + 10.0f, 18, BLACK);

        if (GuiButton({box.x + box.width - 160.0f, box.y + 10.0f, 60.0f, 30.0f}, "Buy")) {
            // Optional: Pre-fill dropdown or trigger auto-buy
        }
        if (GuiButton({box.x + box.width - 80.0f, box.y + 10.0f, 60.0f, 30.0f}, "Sell")) {
            // Sell one share
            if (PlayerData::Instance().SellStock(company, 1)) {
                PlayerData::Instance().cash += company->GetCurrentPrice();
            }
        }
    }

    if (stocks.empty()) {
        DrawText("You don't own any stocks yet. Buy from a company to begin!", 140.0f, 300.0f, 20, GRAY);
    }
}

void Menu::DrawUpgradesScreen()
{
    DrawText("UPGRADES", 140, 70, 30, DARKGRAY);

    Rectangle searchBox = { GetScreenWidth() - 180.0f, 60.0f + 10.0f, 170.0f, 30.0f };

    // Click detection
    if (CheckCollisionPointRec(GetMousePosition(), searchBox)) {
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            isSearchFocused = true;

            // Clear placeholder when first clicked
            if (strcmp(searchText, "Search...") == 0) {
                searchText[0] = '\0'; // Clear text
            }
        }
    } else if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        isSearchFocused = false;
    }

    // Show placeholder if not focused and empty
    if (!isSearchFocused && strlen(searchText) == 0) {
        strcpy(searchText, "Search...");
    }

    // Optional styles
    GuiSetStyle(TEXTBOX, BASE_COLOR_NORMAL, ColorToInt(WHITE));
    GuiSetStyle(TEXTBOX, BORDER_COLOR_NORMAL, ColorToInt(GRAY));
    GuiSetStyle(TEXTBOX, TEXT_COLOR_NORMAL, ColorToInt(BLACK));

    // Render the textbox (editable if focused)
    GuiTextBox(searchBox, searchText, 32, isSearchFocused);

    float boxStartY = 110.0f;
    float boxHeight = 80.0f;
    float boxWidth = GetScreenWidth() - 160.0f;

    std::vector<Upgrade>& upgrades = upgradeHandler.getUpgrades();

    for (size_t i = 0; i < upgrades.size(); ++i) {
        Upgrade& upgrade = upgrades[i];

        float y = boxStartY + i * (boxHeight + 10.0f);
        Rectangle box = {140.0f, y, boxWidth, boxHeight};

        DrawRectangleRec(box, LIGHTGRAY);
        DrawRectangleLinesEx(box, 1, GRAY);

        std::string upgradeInfo = upgrade.getName() + " | $" + std::to_string(upgrade.getCost());

        std::string upgradeDesc = upgrade.getDescription();

        DrawText(upgradeInfo.c_str(), box.x + 10.0f, box.y + 10.0f, 20, BLACK);
        DrawText(upgradeDesc.c_str(), box.x + 10.0f, box.y + 50.0f, 18, BLACK);

        if (GuiButton({box.x + box.width - 120.0f, box.y + 25.0f, 80.0f, 30.0f}, "Buy")) {
            upgradeHandler.handlePurchase(i, player, message);
        }
        message.Draw();
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
