//
// Created by Noah Peñaranda on 7/30/2025.
//

#include "PortfolioScreen.h"
#include "Objects/Layout.h"
#include "raylib.h"
#include "raygui.h"
#include <sstream>
#include <iomanip>
#include <string>
#include <cstring>

#include "Objects/PopUpWindow.h"

void PortfolioScreen::Draw() {
    Layout layout(GetScreenWidth(), GetScreenHeight());

    DrawText("PORTFOLIO", 140, 70, 30, DARKGRAY);

    Rectangle searchBox = { layout.screenWidth - 180.0f, layout.topOffset + 10.0f, 170.0f, 30.0f };

    // Handle search bar logic
    if (CheckCollisionPointRec(GetMousePosition(), searchBox)) {
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            isSearchFocused = true;
            if (strcmp(searchText, "Search...") == 0) searchText[0] = '\0';
        }
    } else if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        isSearchFocused = false;
    }

    if (!isSearchFocused && strlen(searchText) == 0) {
        strcpy(searchText, "Search...");
    }

    GuiSetStyle(TEXTBOX, BASE_COLOR_NORMAL, ColorToInt(WHITE));
    GuiSetStyle(TEXTBOX, BORDER_COLOR_NORMAL, ColorToInt(GRAY));
    GuiSetStyle(TEXTBOX, TEXT_COLOR_NORMAL, ColorToInt(BLACK));
    GuiTextBox(searchBox, searchText, 32, isSearchFocused);

    std::string searchLower = searchText;
    std::transform(searchLower.begin(), searchLower.end(), searchLower.begin(), ::tolower);
    bool searchEmpty = (strcmp(searchText, "Search...") == 0 || searchLower.empty());

    std::vector<Stock*>& stocks = PlayerData::Instance().GetStocks();
    int shownCount = 0;

    for (size_t i = 0; i < stocks.size(); ++i) {
        Stock* stock = stocks[i];
        Company* company = stock->company;
        float shares = stock->shares;

        if (shares < stock->minimumShares) continue;

        std::string nameLower = company->companyName;
        std::transform(nameLower.begin(), nameLower.end(), nameLower.begin(), ::tolower);
        if (!searchEmpty && nameLower.find(searchLower) == std::string::npos) continue;

        float y = layout.GetBoxStartY() + shownCount * (layout.rowHeight + layout.spacing);
        if (y + layout.rowHeight > layout.screenHeight) break;

        Rectangle row = {
            layout.GetBoxX(),
            y,
            layout.GetBoxWidth(),
            layout.rowHeight
        };

        DrawRectangleRec(row, (shownCount % 2 == 0) ? Color{230, 230, 230, 255} : Color{250, 250, 250, 255});
        DrawRectangleLinesEx(row, 1, GRAY);

        float value = stock->GetShareValue();
        float increase = company->CalculateIncreaseFromWeeksAgo(12);

        std::ostringstream info;
        info << company->companyName
             << " | Shares: " << std::fixed << std::setprecision(2) << shares
             << " | Value: $" << std::fixed << std::setprecision(2) << value
             << " | Increase: " << (increase >= 0 ? "+" : "") << increase << "%";

        float textY = row.y + 10.0f;
        DrawText(info.str().c_str(), row.x + 10.0f, textY, 18, BLACK);

        Rectangle buyBtn = {row.x + row.width - 160.0f, row.y + 10.0f, 60.0f, 30.0f};
        Rectangle sellBtn = {row.x + row.width - 80.0f, row.y + 10.0f, 60.0f, 30.0f};

        if (GuiButton(buyBtn, "Buy")) {
            popupCompany = company;
            showPopup = true;
            showBuyPopup = true;
            strcpy(inputBuffer, "0.0");
        }

        if (GuiButton(sellBtn, "Sell")) {
            popupCompany = company;
            showBuyPopup = false; // for sell
            showPopup = true;
            strcpy(inputBuffer, "0.0");
        }

        shownCount++;
    }

    if (shownCount == 0) {
        DrawText("You don't own any stocks yet. Buy from a company to begin!", 140.0f, 300.0f, 20, GRAY);
    }

    // ===== POPUP HANDLER =====
    if (popupCompany) {
        GameState::Instance().SetTempPause(true);
        PopUpWindow().DrawBuySellPopup(showBuyPopup, showPopup, popupCompany, PlayerData::Instance(), inputBuffer);
    }
}
