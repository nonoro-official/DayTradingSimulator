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

void Menu::Init(GameState* gameRef) {
    game = gameRef;
    upgradeHandler.init(*game);

    // Initialize Companies
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

void Menu::DrawDashboardScreen()
{
    Layout layout(GetScreenWidth(), GetScreenHeight());

    // 1. Top Section (Dropdown area)
    Rectangle topBar = { layout.leftOffset, layout.topOffset, layout.screenWidth - layout.leftOffset, layout.sectionHeight };
    DrawRectangleRec(topBar, GRAY);

    // Optional: Set dropdown styles (set once is enough — do this before or in Init())
    GuiSetStyle(DROPDOWNBOX, BASE_COLOR_NORMAL, ColorToInt(WHITE));
    GuiSetStyle(DROPDOWNBOX, BASE_COLOR_PRESSED, ColorToInt(LIGHTGRAY));
    GuiSetStyle(DROPDOWNBOX, BORDER_COLOR_NORMAL, ColorToInt(DARKGRAY));
    GuiSetStyle(DROPDOWNBOX, TEXT_COLOR_NORMAL, ColorToInt(BLACK));
    GuiSetStyle(DROPDOWNBOX, TEXT_COLOR_PRESSED, ColorToInt(BLACK));

    // Dropdown state (persistent between frames)
    static int selectedCompanyIndex = 0;
    static bool dropdownOpen = false;

    // Dropdown options
    const char* companyNames = "Lemon Inc.;Banana Corp;Mango Ltd";

    Rectangle dropdownBounds = { topBar.x + 20, topBar.y + 15, 180, 30 };
    if (CheckCollisionPointRec(GetMousePosition(), dropdownBounds) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        dropdownOpen = !dropdownOpen;
    }

    // 2. Middle Section (Graph)
    Rectangle graphArea = {
        layout.leftOffset,
        topBar.y + layout.sectionHeight,
        layout.screenWidth - layout.leftOffset,
        layout.screenHeight - layout.topOffset - layout.sectionHeight * 2
    };

    if (selectedCompanyIndex >= 0 && selectedCompanyIndex < companies.size() && companies[selectedCompanyIndex]) {
        companies[selectedCompanyIndex]->display->Draw();
    }

    // Dropdown
    GuiDropdownBox(dropdownBounds, companyNames, &selectedCompanyIndex, dropdownOpen);

    // 3. Bottom Section (Buy/Sell)
    Rectangle bottomBar = {
        layout.leftOffset,
        graphArea.y + graphArea.height,
        layout.screenWidth - layout.leftOffset,
        layout.sectionHeight
    };
    DrawRectangleRec(bottomBar, GRAY);

    float buttonWidth = 100;
    float buttonHeight = 30;
    float spacing = 20;
    float buttonY = bottomBar.y + (layout.sectionHeight - buttonHeight) / 2;

    Rectangle buyBtn = { bottomBar.x + 20, buttonY, buttonWidth, buttonHeight };
    Rectangle sellBtn = { buyBtn.x + buttonWidth + spacing, buttonY, buttonWidth, buttonHeight };

    GuiSetStyle(DEFAULT, TEXT_SIZE, 18);

    if (GuiButton(buyBtn, "BUY")) {
        TraceLog(LOG_INFO, "Buy button clicked!");
    }

    if (GuiButton(sellBtn, "SELL")) {
        TraceLog(LOG_INFO, "Sell button clicked!");
    }

    // Info Display (on right side)
    float infoX = sellBtn.x + buttonWidth + spacing * 2;
    float infoY = bottomBar.y + 10;
    int fontSize = 20;

    // Dummy data for now — replace with company info later
    const char* selectedCompany = "Lemon Inc.";
    float price = 52.75f;
    float increase = 12.5f;

    std::ostringstream stream;

    stream << std::fixed << std::setprecision(2);
    stream << "Share Price: $" << price
           << " | Increase: " << (increase >= 0 ? "+" : "") << increase << "%";

    std::string companyInfo = stream.str();

    DrawText(companyInfo.c_str(), infoX + 70, infoY + 10, fontSize, BLACK);
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
    Layout layout(GetScreenWidth(), GetScreenHeight());

    DrawText("COMPANIES", 140, 70, 30, DARKGRAY);

    Rectangle searchBox = { layout.screenWidth - 180.0f, layout.topOffset + 10.0f, 170.0f, 30.0f };

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

    if (companies.empty()) {
        DrawText("No companies found.", 140, 110, 20, RED);
        return;
    }

    // ==== COMPANY LIST ====
    for (size_t i = 0; i < companies.size(); ++i)
    {
        Company* company = companies[i];
        if (!company) continue;

        float y = layout.GetBoxStartY() + i * (layout.rowHeight + layout.spacing);

        Rectangle row = {
            layout.GetBoxX(),
            y,
            layout.GetBoxWidth(),
            layout.rowHeight
        };

        // Skip rows that would go off screen
        if (y + layout.rowHeight > layout.screenHeight) break;

        // Alternate row color
        Color bgColor = (i % 2 == 0) ? Color{ 230, 230, 230, 255 } : Color{ 250, 250, 250, 255 };
        DrawRectangleRec(row, bgColor);
        DrawRectangleLinesEx(row, 1, GRAY);

        // ==== Text Info ====
        std::string name = company->companyName;
        float price = company->GetCurrentPrice();
        float increase = company->CalculateIncrease(0, 0, game->GetMonth(), game->GetWeek());

        // Use stringstream for proper formatting
        std::ostringstream priceInfoStream;
        priceInfoStream << std::fixed << std::setprecision(2);
        priceInfoStream << "share price: $" << price
                        << "  |  change: " << (increase >= 0 ? "+" : "") << increase;

        std::string priceInfo = priceInfoStream.str();

        // Text positions
        float textY = row.y + (layout.rowHeight - 20) / 2;

        DrawText(name.c_str(), row.x + 15, textY, 20, BLACK);
        DrawText(priceInfo.c_str(), row.x + row.width - MeasureText(priceInfo.c_str(), 20) - 15, textY, 20, BLACK);
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
