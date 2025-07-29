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

    Stock* stock = GameState::Instance().GetStockByCompany(company);
    if (!stock) return;

    Rectangle popup = { GetScreenWidth() / 2.0f - 300, GetScreenHeight() / 2.0f - 200, 600, 400 };
    DrawRectangleRec(popup, RAYWHITE);
    DrawRectangleLinesEx(popup, 2, DARKGRAY);

    float pricePerShare = company->GetCurrentPrice();
    float playerBalance = player.cash;
    float ownedShares = stock->shares;
    float minShares = stock->minimumShares;
    float inputValue = std::atof(inputText);
    float sharesToBuy = inputValue / pricePerShare;
    float estimatedValue = inputValue * pricePerShare;

    int y = popup.y + 20;
    const float left = popup.x + 20;

    // Title
    DrawText(isBuyMode ? "ENTER AMOUNT TO INVEST" : "ENTER NUMBER OF SHARES TO SELL", left, y, 24, BLACK);
    y += 40;

    // Input box
    Rectangle inputBox = { left, (float)y, 160, 30 };
    static bool inputFocused = false;
    GuiTextBox(inputBox, inputText, 16, inputFocused);
    if (CheckCollisionPointRec(GetMousePosition(), inputBox) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
        inputFocused = true;
    else if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
        inputFocused = false;
    y += 40;

    // --- Shared Info ---
    DrawText(TextFormat("Price per Share: $%.2f", pricePerShare), left, y, 20, DARKGRAY);
    y += 30;

    if (isBuyMode) {
        DrawText(TextFormat("Approx. Units You Can Buy: %.2f", sharesToBuy), left, y, 20, DARKGRAY);
        y += 30;
        if (ownedShares > 0) {
            DrawText(TextFormat("Current Shares Owned: %.2f", ownedShares), left, y, 20, DARKGRAY);
            y += 30;
        }
        DrawText(TextFormat("Minimum Required to Buy: %.2f shares", minShares), left, y, 20, DARKGRAY);
        y += 30;
        DrawText(TextFormat("Your Balance: $%.2f", playerBalance), left, y, 20, DARKGRAY);
        y += 30;
    } else {
        DrawText(TextFormat("You Own: %.2f Shares", ownedShares), left, y, 20, DARKGRAY);
        y += 30;
        DrawText(TextFormat("Estimated Sale Value: $%.2f", estimatedValue), left, y, 20, DARKGRAY);
        y += 30;
        DrawText(TextFormat("Minimum Remaining: %.2f (or sell all)", minShares), left, y, 20, DARKGRAY);
        y += 30;
    }

    // --- Button Logic ---
    Rectangle confirmBtn = { popup.x + popup.width - 220, popup.y + popup.height - 50, 90, 30 };
    Rectangle cancelBtn  = { popup.x + popup.width - 110, popup.y + popup.height - 50, 90, 30 };

    bool canBuy = inputValue >= 0.01f && inputValue <= playerBalance && sharesToBuy >= minShares;
    bool canSell = inputValue >= 0.01f && inputValue <= ownedShares &&
                   (ownedShares - inputValue >= minShares || fabsf(ownedShares - inputValue) <= 0.001f);

    // Change confirm button to red if invalid
    if ((isBuyMode && !canBuy) || (!isBuyMode && !canSell)) {
        GuiSetStyle(BUTTON, BASE_COLOR_NORMAL, ColorToInt(RED));
    }

    if (GuiButton(confirmBtn, isBuyMode ? "BUY" : "SELL")) {
        if (isBuyMode) {
            if (!canBuy) {
                if (inputValue < 0.01f) Show("Enter a valid investment amount.");
                else if (inputValue > playerBalance) Show("Not enough funds.");
                else Show("Must meet minimum shares requirement.");
            } else {
                stock->BuyStock(sharesToBuy, inputValue);
                Show("Stock purchased!");
                isVisible = false;
                strcpy(inputText, "");
                GameState::Instance().SetTempPause(false);
            }
        } else {
            if (!canSell) {
                if (inputValue < 0.01f) Show("Enter a valid share amount.");
                else if (inputValue > ownedShares) Show("You don't own that many shares.");
                else Show("Selling would drop you below the minimum shares.");
            } else {
                stock->SellStock(inputValue);
                Show("Shares sold!");
                isVisible = false;
                strcpy(inputText, "");
                GameState::Instance().SetTempPause(false);
            }
        }
    }

    // Reset color if changed
    if ((isBuyMode && !canBuy) || (!isBuyMode && !canSell)) {
        GuiSetStyle(BUTTON, BASE_COLOR_NORMAL, ColorToInt(LIGHTGRAY));
    }

    if (GuiButton(cancelBtn, "CANCEL")) {
        isVisible = false;
        strcpy(inputText, "");
        GameState::Instance().SetTempPause(false);
    }
}
