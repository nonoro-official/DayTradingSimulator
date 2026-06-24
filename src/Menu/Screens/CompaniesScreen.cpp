//
// Created by Noah Peñaranda on 7/30/2025.
//

#include "CompaniesScreen.h"
#include "raylib.h"
#include "raygui.h"
#include "Objects/Layout.h"
#include <sstream>
#include <iomanip>
#include <string>
#include <cstring>

CompaniesScreen::CompaniesScreen(std::vector<Company*>* companiesRef, PopUpWindow* popupRef)
    : companies(companiesRef), popup(popupRef) {

    descriptionFont = LoadFontEx("../Fonts/VT323-Regular.ttf", 18, 0, 0); // adjust size as needed
}

void CompaniesScreen::Draw() {
    Layout layout(GetScreenWidth(), GetScreenHeight());

    DrawText("COMPANIES", 140, 70, 30, DARKGRAY);

    // ===== Search Box =====
    Rectangle searchBox = { layout.screenWidth - 180.0f, layout.topOffset + 10.0f, 170.0f, 30.0f };

    if (CheckCollisionPointRec(GetMousePosition(), searchBox)) {
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            isSearchFocused = true;
            if (strcmp(searchText, "Search...") == 0) {
                searchText[0] = '\0';
            }
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

    // ===== Filtered Company List =====
    int shownCount = 0;
    std::string lowerSearch = searchText;
    std::transform(lowerSearch.begin(), lowerSearch.end(), lowerSearch.begin(), ::tolower);
    bool isSearchEmpty = (strcmp(searchText, "Search...") == 0 || lowerSearch.empty());

        layout.rowHeight = 100; // ← Increased height for each panel

    for (size_t i = 0; i < companies->size(); ++i) {
        Company* company = (*companies)[i];
        if (!company) continue;

        std::string lowerName = company->companyName;
        std::transform(lowerName.begin(), lowerName.end(), lowerName.begin(), ::tolower);

        if (!isSearchEmpty && lowerName.find(lowerSearch) == std::string::npos) continue;

        float y = layout.GetBoxStartY() + shownCount * (layout.rowHeight + layout.spacing);
        if (y + layout.rowHeight > layout.screenHeight) break;

        Rectangle row = { layout.GetBoxX(), y, layout.GetBoxWidth(), layout.rowHeight };
        Color bgColor = (shownCount % 2 == 0) ? Color{230, 230, 230, 255} : Color{250, 250, 250, 255};
        DrawRectangleRec(row, bgColor);
        DrawRectangleLinesEx(row, 1, GRAY);

        std::ostringstream priceInfo;
        priceInfo << std::fixed << std::setprecision(2);
        priceInfo << "Share price: $" << company->GetCurrentPrice()
                  << "  |  Change: " << (company->CalculateAverageIncreaseOverWeeks(12) >= 0 ? "+" : "")
                  << company->CalculateAverageIncreaseOverWeeks(12);

        float textY = row.y + 10;
        DrawText(company->companyName.c_str(), row.x + 15, textY, 20, BLACK);

        DrawText(priceInfo.str().c_str(), row.x + row.width - MeasureText(priceInfo.str().c_str(), 20) - 200, textY, 20, BLACK);

        // 🆕 Description line
        const char* desc = company->companyStoreDescription.c_str(); // or however you access it
        Vector2 descPos = { row.x + 15, textY + 36 };
        DrawTextEx(descriptionFont, desc, descPos, 18, 1, DARKGRAY);


        Rectangle buyBtn = {
            row.x + row.width - 100.0f,
            row.y + (layout.rowHeight - 30.0f) / 2.0f,
            80.0f,
            30.0f
        };

        if (GuiButton(buyBtn, "Buy")) {
            popupCompany = company;
            showBuyPopup = true;
            strcpy(inputBuffer, "0.0");
        }

        shownCount++;
    }

    if (shownCount == 0) {
        DrawText("No companies found.", 140, 110, 20, RED);
    }

    if (showBuyPopup && popupCompany) {
        GameState::Instance().SetTempPause(true);
        popup->DrawBuySellPopup(true, showBuyPopup, popupCompany, PlayerData::Instance());

        if (!showBuyPopup) {
            GameState::Instance().SetTempPause(false);
        }
    }

}

CompaniesScreen::~CompaniesScreen() {
    UnloadFont(descriptionFont);

}