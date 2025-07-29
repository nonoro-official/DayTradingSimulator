//
// Created by Noah Peñaranda on 7/29/2025.
//

#include "PopUpWindow.h"
#include "raylib.h"
#include "raygui.h"
#include <cstring>

void PopUpWindow::Show(const std::string& p, float durationSeconds)
{
    popUp = p;
    isVisible = true;
    remainingTime = durationSeconds;
}

void PopUpWindow::Hide()
{
    isVisible = false;
}

void PopUpWindow::Draw()
{
    if (isVisible)
    {
        // Update timer
        remainingTime -= GetFrameTime();
        if (remainingTime <= 0.0f)
        {
            Hide();
            return;
        }

        Rectangle messageRect = {
            GetScreenWidth() / 2.0f - 600 / 2.0f,
            GetScreenHeight() / 2.0f - 400 / 2.0f,
            600,
            400
        };
        DrawRectangleRec(messageRect, LIGHTGRAY);
        DrawRectangleLinesEx(messageRect, 1, DARKGRAY);

        int textWidth = MeasureText(popUp.c_str(), 20);
        DrawText(popUp.c_str(), GetScreenWidth() / 2 - textWidth / 2, GetScreenHeight() / 2 - 10, 20, BLACK);

    }
}

void PopUpWindow::DrawBuySellPopup(bool isBuyMode, bool& isVisible, Company* company, PlayerData& player, char* inputText)
{
    if (!isVisible || !company) return;

    Rectangle popup = { GetScreenWidth() / 2.0f - 300, GetScreenHeight() / 2.0f - 200, 600, 400 };
    DrawRectangleRec(popup, RAYWHITE);
    DrawRectangleLinesEx(popup, 2, DARKGRAY);

    float pricePerShare = company->GetCurrentPrice();
    float playerBalance = player.cash;
    float ownedShares = player.GetTotalShares(company);
    float minShares = player.GetMinimumShares(company);  // make sure this is accessible
    float inputValue = std::atof(inputText);
    float estimatedCostOrValue = isBuyMode ? inputValue : inputValue * pricePerShare;

    int y = popup.y + 20;

    DrawText(isBuyMode ? "ENTER AMOUNT TO INVEST" : "ENTER AMOUNT OF SHARES TO SELL", popup.x + 20, y, 24, BLACK);
    y += 50;

    // Input textbox
    Rectangle inputBox = { popup.x + 20, (float)y, 160, 30 };
    static bool inputFocused = false;
    GuiTextBox(inputBox, inputText, 16, inputFocused);
    if (CheckCollisionPointRec(GetMousePosition(), inputBox) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
        inputFocused = true;
    else if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
        inputFocused = false;
    y += 50;

    // Show info
    if (isBuyMode) {
        DrawText(TextFormat("Approximate Units: %.2f", inputValue / pricePerShare), popup.x + 20, y, 20, DARKGRAY); y += 30;
    } else {
        DrawText(TextFormat("Approximate Value: $%.2f", estimatedCostOrValue), popup.x + 20, y, 20, DARKGRAY); y += 30;
    }

    if (!isBuyMode && ownedShares > 0) {
        DrawText(TextFormat("Owned Shares: %.2f", ownedShares), popup.x + 20, y, 20, DARKGRAY); y += 30;
    }

    if (isBuyMode && ownedShares > 0) {
        DrawText(TextFormat("Current Shares: %.2f", ownedShares), popup.x + 20, y, 20, DARKGRAY); y += 30;
    }

    DrawText(TextFormat("Minimum Shares Required: %.2f", minShares), popup.x + 20, y, 20, DARKGRAY); y += 30;
    DrawText(TextFormat("Balance: $%.2f", playerBalance), popup.x + 20, y, 20, DARKGRAY); y += 30;

    // Buttons
    Rectangle confirmBtn = { popup.x + popup.width - 220, popup.y + popup.height - 50, 90, 30 };
    Rectangle cancelBtn  = { popup.x + popup.width - 110, popup.y + popup.height - 50, 90, 30 };

    if (GuiButton(confirmBtn, isBuyMode ? "BUY" : "SELL")) {
        if (isBuyMode) {
            if (inputValue < 0.01f) {
                Show("Enter a valid investment amount.");
            } else if (inputValue > playerBalance) {
                Show("Not enough funds.");
            } else {
                Stock stock(*company, minShares);
                stock.shares = inputValue / pricePerShare;
                player.AddStock(stock);
                player.cash -= inputValue;
                Show("Stock purchased!");
                isVisible = false;
                strcpy(inputText, "");
            }
        } else {
            if (inputValue > ownedShares) {
                Show("You don't own that many shares.");
            } else if (inputValue < 0.01f) {
                Show("Enter a valid share amount.");
            } else {
                player.SellStock(company, inputValue);
                player.cash += estimatedCostOrValue;
                Show("Shares sold!");
                isVisible = false;
                strcpy(inputText, "");
            }
        }
    }

    if (GuiButton(cancelBtn, "CANCEL")) {
        isVisible = false;
        strcpy(inputText, "");
        GameState::Instance().SetTempPause(false);
    }
}