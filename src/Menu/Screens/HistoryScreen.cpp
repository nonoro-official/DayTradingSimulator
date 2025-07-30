//
// Created by Noah Peñaranda on 7/30/2025.
//

#include "HistoryScreen.h"
#include "Objects/Layout.h"
#include "raylib.h"
#include "raygui.h"
#include <sstream>
#include <iomanip>
#include <string>
#include <cstring>
void HistoryScreen::Draw() {
    Layout layout(GetScreenWidth(), GetScreenHeight());

    DrawText("TRANSACTION HISTORY", 140, 70, 30, DARKGRAY);

    // --- Filter dropdown state
    static bool dropdownOpen = false;
    const char* filterOptions[] = { "All", "Buy", "Sell", "Placed Buy", "Placed Sell" };
    const int optionCount = sizeof(filterOptions) / sizeof(filterOptions[0]);

    // --- Search Box
    Rectangle searchBox = { layout.screenWidth - 180.0f, layout.topOffset + 10.0f, 170.0f, 30.0f };

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

    // Prepare search and filter
    std::string searchLower = searchText;
    std::transform(searchLower.begin(), searchLower.end(), searchLower.begin(), ::tolower);
    bool searchEmpty = (strcmp(searchText, "Search...") == 0 || searchLower.empty());

    const std::vector<TransactionRecord>& history = TransactionManager::Instance().GetHistory();
    int shownCount = 0;

    for (int i = history.size() - 1; i >= 0; --i) {
        const TransactionRecord& record = history[i];

        std::string companyName = record.stock->company->companyName;
        std::string nameLower = companyName;
        std::transform(nameLower.begin(), nameLower.end(), nameLower.begin(), ::tolower);

        // --- Apply filter
        if (selectedType != 0 && static_cast<int>(record.type) != selectedType - 1)
            continue;

        if (!searchEmpty && nameLower.find(searchLower) == std::string::npos)
            continue;

        float y = layout.GetBoxStartY() + shownCount * (layout.rowHeight + layout.spacing);
        if (y + layout.rowHeight > layout.screenHeight) break;

        Rectangle row = {
            layout.GetBoxX(),
            y + 30.0f,
            layout.GetBoxWidth(),
            layout.rowHeight
        };

        // --- Background color based on type
        Color bgColor;
        switch (record.type) {
            case TransactionRecord::Buy:
                bgColor = Color{220, 255, 220, 255}; // light green
                break;
            case TransactionRecord::Sell:
                bgColor = Color{255, 220, 220, 255}; // light red
                break;
            case TransactionRecord::PlacedBuy:
                bgColor = Color{220, 240, 255, 255}; // light blue
                break;
            case TransactionRecord::PlacedSell:
                bgColor = Color{255, 240, 200, 255}; // light orange
                break;
            default:
                bgColor = Color{245, 245, 245, 255}; // fallback
                break;
        }

        DrawRectangleRec(row, bgColor);
        DrawRectangleLinesEx(row, 1, GRAY);

        // --- Text color based on type
        Color textColor;
        switch (record.type) {
            case TransactionRecord::Buy:
                textColor = DARKGREEN;
                break;
            case TransactionRecord::Sell:
                textColor = MAROON;
                break;
            case TransactionRecord::PlacedBuy:
                textColor = DARKBLUE;
                break;
            case TransactionRecord::PlacedSell:
                textColor = ORANGE;
                break;
            default:
                textColor = BLACK;
                break;
        }

        // --- Text content
        std::ostringstream info;
        info << (record.type == TransactionRecord::Buy ? "BUY" :
                 record.type == TransactionRecord::Sell ? "SELL" :
                 record.type == TransactionRecord::PlacedBuy ? "PLACED BUY" : "PLACED SELL");

        info << " | " << companyName
             << " | Shares: " << std::fixed << std::setprecision(2) << record.shares
             << " | Price: $" << record.pricePerShare
             << " | Total: $" << record.totalAmount;

        std::ostringstream dateStr;
        dateStr << "Month " << record.month << ", Week " << record.week;

        float textY = row.y + 10.0f;
        DrawText(info.str().c_str(), row.x + 10.0f, textY, 18, textColor);
        DrawText(dateStr.str().c_str(), row.x + 10.0f, textY + 25.0f, 16, DARKGRAY);

        shownCount++;
    }

    if (shownCount == 0) {
        DrawText("No transactions found.", 140, 160, 20, RED);
    }

    Rectangle dropdownBounds = { 205, 110, 150, 30 };
    GuiSetStyle(DROPDOWNBOX, BASE_COLOR_NORMAL, ColorToInt(WHITE));

    // Toggle dropdown on click
    if (CheckCollisionPointRec(GetMousePosition(), dropdownBounds) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        dropdownOpen = !dropdownOpen;
    }

    // Draw selected item
    DrawText("Filter: ", 140, 115, 20, DARKGRAY);
    DrawRectangleRec(dropdownBounds, LIGHTGRAY);
    DrawText(filterOptions[selectedType], dropdownBounds.x + 10, dropdownBounds.y + 7, 20, BLACK);
    DrawRectangleLinesEx(dropdownBounds, 1, DARKGRAY);

    // Draw dropdown options
    if (dropdownOpen) {
        for (int i = 0; i < optionCount; ++i) {
            Rectangle optionBounds = {
                dropdownBounds.x,
                dropdownBounds.y + (i + 1) * dropdownBounds.height,
                dropdownBounds.width,
                dropdownBounds.height
            };

            bool isHovered = CheckCollisionPointRec(GetMousePosition(), optionBounds);
            Color hoverBlue = Color{ 173, 216, 230, 255 }; // Light blue

            DrawRectangleRec(optionBounds, isHovered ? hoverBlue : RAYWHITE);
            DrawRectangleLinesEx(optionBounds, 1, GRAY);
            DrawText(filterOptions[i], optionBounds.x + 10, optionBounds.y + 7, 20, BLACK);

            if (isHovered && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                selectedType = i;
                dropdownOpen = false;
            }
        }
    }
}
