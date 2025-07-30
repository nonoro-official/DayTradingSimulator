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

    Rectangle searchBox = { layout.screenWidth - 180.0f, layout.topOffset + 10.0f, 170.0f, 30.0f };

    // --- Search box behavior
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

    const std::vector<TransactionRecord>& history = TransactionManager::Instance().GetHistory();
    int shownCount = 0;

    for (int i = history.size() - 1; i >= 0; --i) {
        const TransactionRecord& record = history[i];

        std::string companyName = record.stock->company->companyName;
        std::string nameLower = companyName;
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

        Color bgColor = (shownCount % 2 == 0) ? Color{ 245, 245, 245, 255 } : Color{ 255, 255, 255, 255 };
        DrawRectangleRec(row, bgColor);
        DrawRectangleLinesEx(row, 1, GRAY);

        // Format info line
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
        DrawText(info.str().c_str(), row.x + 10.0f, textY, 18, BLACK);
        DrawText(dateStr.str().c_str(), row.x + 10.0f, textY + 25.0f, 16, DARKGRAY);

        shownCount++;
    }

    if (shownCount == 0) {
        DrawText("No transactions found.", 140, 140, 20, RED);
    }
}