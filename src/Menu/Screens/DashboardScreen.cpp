//
// Created by Noah Peñaranda on 7/30/2025.
//

#include "DashboardScreen.h"
#include "Objects/Layout.h"
#include "raylib.h"
#include "raygui.h"
#include <sstream>
#include <iomanip>
#include <cstring>

void DashboardScreen::UpdatePrediction()
{
    prediction = PlayerData::Instance().GetMarketPrediction(GameState::Instance().GetCompanyByIndex(*selectedCompanyIndex));
}

DashboardScreen::DashboardScreen(std::vector<Company*>* companiesRef, int* selectedIndex, PopUpWindow* popupRef)
    : companies(companiesRef), selectedCompanyIndex(selectedIndex), popup(popupRef)
{
    prediction = PlayerData::Instance().GetMarketPrediction(GameState::Instance().GetCompanyByIndex(*selectedCompanyIndex));

    GameState::Instance().AddTickListener([this]()
    {
        UpdatePrediction();
    });
}


std::string DashboardScreen::BuildCompanyDropdownString()
{
    std::string result;
    for (size_t i = 0; i < companies->size(); ++i)
    {
        result += (*companies)[i]->GetName();
        if (i != companies->size() - 1)
            result += ";";
    }
    return result;
}

void DashboardScreen::Update()
{
    for (size_t i = 0; i < companies->size(); ++i)
    {
        (*companies)[i]->display->active = (i == *selectedCompanyIndex);
        (*companies)[i]->display->Update();
    }
}

bool DashboardScreen::CanTrade(Company *company)
{
    if (!company)
        return false;

    Stock *stock = GameState::Instance().GetStockByCompany(company);

    // Optional sanity check (e.g., not enough cash or shares)
    if (PlayerData::Instance().cash < 0.01f && stock->shares < 0.01f)
    {
        popup->Show("You don't have enough funds or shares to trade.");
        return false;
    }

    return true;
}

void DashboardScreen::Draw()
{
    Layout layout(GetScreenWidth(), GetScreenHeight());

    Company *selectedCompany = nullptr;
    if (*selectedCompanyIndex >= 0 && *selectedCompanyIndex < (int)companies->size())
    {
        selectedCompany = (*companies)[*selectedCompanyIndex];
    }

    // 1. Top Bar Section (Dropdown Area)
    Rectangle topBar = {
        layout.leftOffset,
        layout.topOffset,
        layout.screenWidth - layout.leftOffset,
        layout.sectionHeight};
    DrawRectangleRec(topBar, GRAY);

    // --- 2. Graph Section
    Rectangle graphArea = {
        layout.leftOffset,
        topBar.y + layout.sectionHeight,
        layout.screenWidth - layout.leftOffset,
        layout.screenHeight - layout.topOffset - layout.sectionHeight * 2};

    if (*selectedCompanyIndex >= 0 && *selectedCompanyIndex < (int)companies->size())
    {
        Company *selectedCompany = (*companies)[*selectedCompanyIndex];
        selectedCompany->display->Draw();
        selectedCompany->display->DrawTooltips();
    }
    GuiSetStyle(DROPDOWNBOX, BASE_COLOR_NORMAL, ColorToInt(WHITE));

    // Build the dropdown string fresh each frame (in case companies update)
    cachedDropdown = BuildCompanyDropdownString();
    const char *companyItems = cachedDropdown.c_str();

    Rectangle dropdownBounds = {topBar.x + 20, topBar.y + 15, 180, 30};

    // Toggle open if clicked inside
    if (!dropdownOpen && IsMouseButtonPressed(MOUSE_LEFT_BUTTON) &&
        CheckCollisionPointRec(GetMousePosition(), dropdownBounds))
    {
        dropdownOpen = true;
    }

    // Use temp index to detect changes (but don't close on select)
    int tempIndex = *selectedCompanyIndex;
    GuiDropdownBox(dropdownBounds, companyItems, &tempIndex, dropdownOpen);

    // Update selection (but do not close dropdown)
    if (*selectedCompanyIndex != tempIndex)
    {
        *selectedCompanyIndex = tempIndex;

        UpdatePrediction();
    }

    // Close dropdown only when clicking outside
    if (dropdownOpen && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
    {
        Vector2 mouse = GetMousePosition();
        Rectangle fullDropdownArea = {
            dropdownBounds.x,
            dropdownBounds.y,
            dropdownBounds.width,
            dropdownBounds.height * ((int)companies->size() + 1) // +1 for header
        };

        if (!CheckCollisionPointRec(mouse, fullDropdownArea))
        {
            dropdownOpen = false;
        }
    }

    // --- 3. Bottom Bar (Buy/Sell + Info)
    Rectangle bottomBar = {
        layout.leftOffset,
        graphArea.y + graphArea.height,
        layout.screenWidth - layout.leftOffset,
        layout.sectionHeight};
    DrawRectangleRec(bottomBar, GRAY);

    float buttonWidth = 100;
    float buttonHeight = 30;
    float spacing = 20;
    float buttonY = bottomBar.y + (layout.sectionHeight - buttonHeight) / 2;

    Rectangle buyBtn = {bottomBar.x + 20, buttonY, buttonWidth, buttonHeight};
    Rectangle sellBtn = {buyBtn.x + buttonWidth + spacing, buttonY, buttonWidth, buttonHeight};

    GuiSetStyle(DEFAULT, TEXT_SIZE, 18);

    if (GuiButton(buyBtn, "BUY"))
    {
        if (selectedCompany && CanTrade(selectedCompany))
        {
            showBuyPopup = true;
            strcpy(inputBuffer, "0.0");
        }
    }

    if (GuiButton(sellBtn, "SELL"))
    {
        if (selectedCompany && CanTrade(selectedCompany))
        {
            showSellPopup = true;
            strcpy(inputBuffer, "0.0");
        }
    }

    // --- Company Info Text
    float infoX = sellBtn.x + buttonWidth + spacing * 2;
    float infoY = bottomBar.y + 10;
    int fontSize = 20;

    // Add this before the popup logic


    if (*selectedCompanyIndex >= 0 && *selectedCompanyIndex < (int)companies->size())
    {
        selectedCompany = (*companies)[*selectedCompanyIndex];

        float price = selectedCompany->GetCurrentPrice();
        float increase = selectedCompany->CalculateAverageIncreaseOverWeeks(12);

        std::ostringstream stream;
        stream << std::fixed << std::setprecision(2);
        stream << "Share Price: $" << price
               << " | Increase: " << (increase >= 0 ? "+" : "") << increase << "%";

        std::string companyInfo = stream.str();
        int infoWidth = MeasureText(companyInfo.c_str(), fontSize);
        float rightPadding = 20.0f;
        float infoXRight = layout.screenWidth - infoWidth - rightPadding;
        float infoYCentered = bottomBar.y + (layout.sectionHeight - fontSize) / 2;

        DrawText(companyInfo.c_str(), infoXRight, infoYCentered, fontSize, BLACK);
    }

    // Now this will work since selectedCompany is in scope
    if (showBuyPopup && selectedCompany)
    {
        GameState::Instance().SetTempPause(true);
        popup->DrawBuySellPopup(true, showBuyPopup, selectedCompany, PlayerData::Instance());
    }
    if (showSellPopup && selectedCompany)
    {
        GameState::Instance().SetTempPause(true);
        popup->DrawBuySellPopup(false, showSellPopup, selectedCompany, PlayerData::Instance());
    }

    // Draw persistent popup messages (success/error)
    popup->Draw();

    // Prediction hint
    if (selectedCompany)
    {
        int predictionFontSize = 18;

        // Measure prediction width so we can align it to the right
        int textWidth = MeasureText(prediction.c_str(), predictionFontSize);
        float padding = 20.0f;
        float textX = layout.screenWidth - textWidth - padding;
        float textY = topBar.y + (layout.sectionHeight - predictionFontSize) / 2;

        DrawText(prediction.c_str(), textX, textY, predictionFontSize, BLACK);
    }
}
