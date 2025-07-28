//
// Created by Noah Peñaranda on 7/24/2025.
//

#include "Menu.h"
#include "raylib.h"
#include "raygui.h"
#include "Classes/Company.h"
#include "Classes/Stock.h"
#include "Classes/PlayerData.h"
#include <cstring>

void Menu::Init(GameState* gameRef) {
    game = gameRef;
    upgradeHandler.init(*game);

    companies.push_back(new Company("Lemon Inc", "Citrus Tech Giant", 50.0f, 1.0f, 0.75f, new GraphDisplay({540, 300}, {840, 360})));
    companies.push_back(new Company("Banana Corp", "AI + Fruit", 35.0f, 1.1f, 0.65f, new GraphDisplay({540, 300}, {840, 360})));
    companies.push_back(new Company("Mango Ltd", "Tropical Logistics", 45.0f, 0.9f, 0.85f, new GraphDisplay({540, 300}, {840, 360})));
}

void Menu::Update() {
    if (currentScreen == SCREEN_DASHBOARD && !game->IsPaused()) {
        for (Company* c: companies) {
            c->display->Update();
        }
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

void Menu::Draw()
{
    switch (currentScreen)
    {
    case SCREEN_DASHBOARD:
        // graphDisplay->Draw();
        DrawDashboardScreen();
        break;
    case SCREEN_PORTFOLIO:
        DrawPortfolioScreen();
        break;
    case SCREEN_COMPANIES:
        DrawCompaniesScreen();
        break;
    case SCREEN_UPGRADES:
        DrawUpgradesScreen();
        break;
    }
}

// Dropdown state (persistent between frames)
static int selectedCompanyIndex = 0;
static bool dropdownOpen = false;

void Menu::DrawDashboardScreen() {
    const float topOffset = 60.0f;   // your existing global top bar
    const float leftOffset = 120.0f; // width of your sidebar
    const float sectionHeight = 60.0f; // height of top/bottom dashboard bars

    const int screenWidth = 960;
    const int screenHeight = 540;

    // 1. Top Section (Dropdown area)
    Rectangle topBar = { leftOffset, topOffset, screenWidth - leftOffset, sectionHeight };
    DrawRectangleRec(topBar, GRAY);

    // Optional: Set dropdown styles (set once is enough — do this before or in Init())
    GuiSetStyle(DROPDOWNBOX, BASE_COLOR_NORMAL, ColorToInt(WHITE));
    GuiSetStyle(DROPDOWNBOX, BASE_COLOR_PRESSED, ColorToInt(LIGHTGRAY));
    GuiSetStyle(DROPDOWNBOX, BORDER_COLOR_NORMAL, ColorToInt(DARKGRAY));
    GuiSetStyle(DROPDOWNBOX, TEXT_COLOR_NORMAL, ColorToInt(BLACK));
    GuiSetStyle(DROPDOWNBOX, TEXT_COLOR_PRESSED, ColorToInt(BLACK));

    // Dropdown options (semicolon-separated)
    const char* companyNames = "Lemon Inc.;Banana Corp;Mango Ltd";

    // Dropdown position
    Rectangle dropdownBounds = { topBar.x + 20, topBar.y + 15, 180, 30 };

    // Toggle manually
    if (CheckCollisionPointRec(GetMousePosition(), dropdownBounds) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        dropdownOpen = !dropdownOpen;
    }

    // 2. Middle Section (Graph)
    Rectangle graphArea = {leftOffset, topBar.y + sectionHeight, screenWidth - leftOffset, screenHeight - topOffset - sectionHeight * 2};

    // Draw the actual graph
    companies[selectedCompanyIndex]->display->Draw();

    // Draw dropdown
    GuiDropdownBox(dropdownBounds, companyNames, &selectedCompanyIndex, dropdownOpen);

    // 3. Bottom Section (Buy/Sell info)
    Rectangle bottomBar = {leftOffset, graphArea.y + graphArea.height, screenWidth - leftOffset, sectionHeight};
    DrawRectangleRec(bottomBar, GRAY);

    // Button positions
    float buttonWidth = 100;
    float buttonHeight = 30;
    float spacing = 20;
    float buttonY = bottomBar.y + (sectionHeight - buttonHeight) / 2;

    Rectangle buyBtn = { bottomBar.x + 20, buttonY, buttonWidth, buttonHeight };
    Rectangle sellBtn = { bottomBar.x + 20 + buttonWidth + spacing, buttonY, buttonWidth, buttonHeight };

    // Set bigger font size for buttons
    GuiSetStyle(DEFAULT, TEXT_SIZE, 18);

    // Buttons
    if (GuiButton(buyBtn, "BUY")) {
        // TODO: Trigger buy logic
        TraceLog(LOG_INFO, "Buy button clicked!");
    }

    if (GuiButton(sellBtn, "SELL")) {
        // TODO: Trigger sell logic
        TraceLog(LOG_INFO, "Sell button clicked!");
    }

    // INFO LABELS on the right
    float infoX = sellBtn.x + buttonWidth + spacing * 2;
    float infoY = bottomBar.y + 10; // Top padding
    int fontSize = 20;

    // Dummy data (replace with actual selected company/player info)
    const char* selectedCompany = "Lemon Inc.";
    float price = 52.75f;
    float increase = 12.5f;

    std::string companyInfo = "Share Price: $" + std::to_string(price) +
                        " | Increase: " + std::to_string(increase);

    DrawText(companyInfo.c_str(), infoX + 70, infoY + 10, fontSize, BLACK);
}

// Search variables for other screens
static char searchText[32] = "Search...";
static bool isSearchFocused = false;

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
        float value = stock.GetShareValue();
        float increase = company->CalculateIncrease(0, 0, game->GetMonth(), game->GetWeek());

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

void Menu::DrawCompaniesScreen()
{
    DrawText("COMPANIES", 140, 70, 30, DARKGRAY);

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
        float value = stock.GetShareValue();
        float increase = company->CalculateIncrease(0, 0, game->GetMonth(), game->GetWeek());

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

    std::vector<Stock>& stocks = PlayerData::Instance().GetStocks();

    for (size_t i = 0; i < stocks.size(); ++i) {
        Stock& stock = stocks[i];
        Company* company = stock.company;
        float shares = stock.shares;
        float value = stock.GetShareValue();
        float increase = company->CalculateIncrease(0, 0, game->GetMonth(), game->GetWeek());

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
