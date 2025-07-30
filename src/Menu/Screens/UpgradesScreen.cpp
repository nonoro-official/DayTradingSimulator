//
// Created by Noah Peñaranda on 7/30/2025.
//

#include "UpgradesScreen.h"
#include "raylib.h"
#include "raygui.h"
#include "Objects/Layout.h"
#include <sstream>
#include <iomanip>
#include <string>
#include <cstring>

#include "CompaniesScreen.h"

UpgradesScreen::UpgradesScreen(UpgradeHandler* handlerRef, PlayerData* playerRef, PopUpWindow* popupRef)
    : handler(handlerRef), player(playerRef), popup(popupRef) {

    descriptionFont = LoadFontEx("../Fonts/VT323-Regular.ttf", 18, 0, 0); // adjust size as needed
}

void UpgradesScreen::Draw()
{
    Layout layout(GetScreenWidth(), GetScreenHeight());

    // ===== Title =====
    DrawText("UPGRADES", layout.leftOffset + 20.0f, layout.topOffset + 10.0f, 30, DARKGRAY);

    // ===== Search Box =====
    Rectangle searchBox = {layout.screenWidth - 180.0f,layout.topOffset + 10.0f, 170.0f,30.0f};

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

    // ===== Upgrade List =====
    std::string searchLower = searchText;
    std::transform(searchLower.begin(), searchLower.end(), searchLower.begin(), ::tolower);
    bool searchEmpty = (strcmp(searchText, "Search...") == 0 || searchLower.empty());

    std::vector<Upgrade>& upgrades = handler->getUpgrades();

    int shownCount = 0;
    for (size_t i = 0; i < upgrades.size(); ++i) {
        Upgrade& upgrade = upgrades[i];

        std::string nameLower = upgrade.getName();
        std::transform(nameLower.begin(), nameLower.end(), nameLower.begin(), ::tolower);

        if (!searchEmpty && nameLower.find(searchLower) == std::string::npos) continue;

        float y = layout.GetBoxStartY() + shownCount * (layout.rowHeight + layout.spacing);

        // Prevent drawing past screen
        if (y + layout.rowHeight > layout.screenHeight) break;

        Rectangle row = {layout.GetBoxX(), y, layout.GetBoxWidth(), layout.rowHeight};

        Color bgColor = (shownCount % 2 == 0) ? Color{230, 230, 230, 255} : Color{250, 250, 250, 255};
        DrawRectangleRec(row, bgColor);
        DrawRectangleLinesEx(row, 1, GRAY);

        std::ostringstream upgradeInfo;
        upgradeInfo << std::fixed << std::setprecision(2);
        upgradeInfo << upgrade.getName() << " | $" << upgrade.getCost()
                    << " | Tier " << upgrade.getTier() << "/" << upgrade.getMaxTier();

        float textY = row.y + 10.0f;
        DrawText(upgradeInfo.str().c_str(), row.x + 10.0f, textY, 20, BLACK);
        Vector2 descPos = { row.x + 10.0f, textY + 24.0f };
        std::string dynamicDescription;

        if (upgrade.getName() == "Faster Execution") {
            dynamicDescription = upgrade.getDescription() + "Current delay: "
                + std::to_string(player->weekExecutionDelay) + " weeks";
        }
        else if (upgrade.getName() == "Prediction Hint") {
            dynamicDescription = upgrade.getDescription() + "Current level: "
                + std::to_string(player->showPredictionTier);
        }
        else if (upgrade.getName() == "Sell Bonus") {
            float percent = (player->sellBonusMultiplier - 1.0f) * 100.0f;
            std::ostringstream bonusStream;
            bonusStream << std::fixed << std::setprecision(1) << percent;

            dynamicDescription = upgrade.getDescription() + "Current bonus: " + bonusStream.str() + "%";
        }
        else {
            dynamicDescription = upgrade.getDescription(); // fallback
        }

        DrawTextEx(descriptionFont, dynamicDescription.c_str(), descPos, 18, 1, DARKGRAY);

        Rectangle buyBtn = {row.x + row.width - 100.0f, row.y + (layout.rowHeight - 30.0f) / 2.0f, 80.0f,30.0f};

        bool isMaxed = upgrade.getTier() >= upgrade.getMaxTier();
        if (isMaxed) {
            GuiDisable();
        }
        if (GuiButton(buyBtn, isMaxed ? "Maxed" : "Buy")) {
            if (!isMaxed)
                handler->handlePurchase(i, *player, *popup);
        }
        if (isMaxed) {
            GuiEnable();
        }

        shownCount++;
    }

    if (shownCount == 0) {
        DrawText("No matching upgrades found.", layout.leftOffset, layout.GetBoxStartY(), 20, RED);
    }

    popup->Draw();
}

UpgradesScreen::~UpgradesScreen() {
    UnloadFont(descriptionFont);
}
