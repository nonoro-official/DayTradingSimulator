//
// Created by Noah Peñaranda on 7/30/2025.
//

#include "DashboardScreen.h"
#include "Objects/Layout.h"
#include "raylib.h"
#include "raygui.h"
#include <sstream>
#include <iomanip>

DashboardScreen::DashboardScreen(std::vector<Company*>* companiesRef, int* selectedIndex)
    : companies(companiesRef), selectedCompanyIndex(selectedIndex) {}

std::string DashboardScreen::BuildCompanyDropdownString() {
    std::string result;
    for (size_t i = 0; i < companies->size(); ++i) {
        result += (*companies)[i]->GetName();
        if (i != companies->size() - 1) result += ";";
    }
    return result;
}

void DashboardScreen::Update() {
    for (size_t i = 0; i < companies->size(); ++i) {
        (*companies)[i]->display->active = (i == *selectedCompanyIndex);
        (*companies)[i]->display->Update();
    }
}

void DashboardScreen::Draw() {
    Layout layout(GetScreenWidth(), GetScreenHeight());

    // 1. Top Bar Section (Dropdown Area)
    Rectangle topBar = {
        layout.leftOffset,
        layout.topOffset,
        layout.screenWidth - layout.leftOffset,
        layout.sectionHeight
    };
    DrawRectangleRec(topBar, GRAY);

    // --- 2. Graph Section
    Rectangle graphArea = {
        layout.leftOffset,
        topBar.y + layout.sectionHeight,
        layout.screenWidth - layout.leftOffset,
        layout.screenHeight - layout.topOffset - layout.sectionHeight * 2
    };

    if (*selectedCompanyIndex >= 0 && *selectedCompanyIndex < (int)companies->size()) {
        Company* selectedCompany = (*companies)[*selectedCompanyIndex];
        selectedCompany->display->Draw();
        selectedCompany->display->DrawTooltips();
    }

    GuiSetStyle(DROPDOWNBOX, BASE_COLOR_NORMAL, ColorToInt(WHITE));
    //     GuiSetStyle(DROPDOWNBOX, BASE_COLOR_PRESSED, ColorToInt(LIGHTGRAY));
    //     GuiSetStyle(DROPDOWNBOX, BORDER_COLOR_NORMAL, ColorToInt(DARKGRAY));
    //     GuiSetStyle(DROPDOWNBOX, TEXT_COLOR_NORMAL, ColorToInt(BLACK));
    //     GuiSetStyle(DROPDOWNBOX, TEXT_COLOR_PRESSED, ColorToInt(BLACK));

    // Build the dropdown string fresh each frame (in case companies update)
    std::string cachedDropdown = BuildCompanyDropdownString();
    const char* companyItems = cachedDropdown.c_str();

    Rectangle dropdownBounds = { topBar.x + 20, topBar.y + 15, 180, 30 };

    float deltaTime = GetFrameTime();
    if (dropdownOpen) dropdownOpenTime += deltaTime;

    // Dropdown interaction
    if (!dropdownOpen && IsMouseButtonPressed(MOUSE_LEFT_BUTTON) &&
        CheckCollisionPointRec(GetMousePosition(), dropdownBounds)) {
        dropdownOpen = true;
        dropdownOpenTime = 0.0f;
        }

    bool itemSelected = GuiDropdownBox(dropdownBounds, companyItems, selectedCompanyIndex, dropdownOpen);

    if (dropdownOpen && IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && dropdownOpenTime > 0.15f) {
        Vector2 mouse = GetMousePosition();
        Rectangle fullDropdownArea = {
            dropdownBounds.x,
            dropdownBounds.y,
            dropdownBounds.width,
            dropdownBounds.height * (int)companies->size()
        };

        if (!CheckCollisionPointRec(mouse, fullDropdownArea)) {
            dropdownOpen = false;
        }
    }

    // --- 3. Bottom Bar (Buy/Sell + Info)
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

    // --- Company Info Text
    float infoX = sellBtn.x + buttonWidth + spacing * 2;
    float infoY = bottomBar.y + 10;
    int fontSize = 20;

    if (*selectedCompanyIndex >= 0 && *selectedCompanyIndex < (int)companies->size()) {
        Company* selectedCompany = (*companies)[*selectedCompanyIndex];
        float price = selectedCompany->GetCurrentPrice();
        float increase = selectedCompany->CalculateIncreaseFromWeeksAgo(1);

        std::ostringstream stream;
        stream << std::fixed << std::setprecision(2);
        stream << "Share Price: $" << price
               << " | Increase: " << (increase >= 0 ? "+" : "") << increase << "%";

        std::string companyInfo = stream.str();
        DrawText(companyInfo.c_str(), infoX + 70, infoY + 10, fontSize, BLACK);
    }
}
