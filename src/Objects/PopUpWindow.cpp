//
// Created by Noah Peñaranda on 7/29/2025.
//

#include "PopUpWindow.h"
#include "raylib.h"
#include "raygui.h"
#include <cstring>
#include <sstream>

#include "Classes/Transactions/TransactionManager.h"

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

const char* PopUpWindow::GetInputBuffer() const
{
    return inputBuffer;
}

void PopUpWindow::Draw()
{
    if (!isVisible) return;

    // Update timer
    remainingTime -= GetFrameTime();
    if (remainingTime <= 0.0f)
    {
        Hide();
        return;
    }

    // === Line parsing ===
    std::istringstream stream(popUp);
    std::vector<std::string> lines;
    std::string line;
    while (std::getline(stream, line)) {
        lines.push_back(line);
    }

    int fontSize = 20;
    float lineSpacing = 1.5f;
    float textPadding = 20.0f;

    // === Calculate max line width and total height ===
    int maxLineWidth = 0;
    for (const auto& l : lines) {
        int w = MeasureText(l.c_str(), fontSize);
        if (w > maxLineWidth) maxLineWidth = w;
    }

    int numLines = lines.size();
    float textHeight = numLines * fontSize * lineSpacing;
    float popupWidth = maxLineWidth + textPadding * 2;
    float popupHeight = textHeight + textPadding * 2;

    Rectangle messageRect = {
        GetScreenWidth() / 2.0f - popupWidth / 2.0f,
        GetScreenHeight() / 2.0f - popupHeight / 2.0f,
        popupWidth,
        popupHeight
    };

    // === Determine popup background color based on content ===
    Color bgColor = LIGHTGRAY;
    if (popUp.find("successful") != std::string::npos) bgColor = Color{ 200, 255, 200, 255 }; // Light green
    else if (popUp.find("not") != std::string::npos || popUp.find("Invalid") != std::string::npos || popUp.find("Could") != std::string::npos)
        bgColor = Color{ 255, 220, 220, 255 }; // Light red
    else if (popUp.find("Enter") != std::string::npos)
        bgColor = Color{ 255, 245, 200, 255 }; // Light yellow

    // === Draw background and border ===
    DrawRectangleRec(messageRect, bgColor);
    DrawRectangleLinesEx(messageRect, 2, DARKGRAY);

    // === Draw centered text line-by-line ===
    float startY = messageRect.y + (popupHeight - textHeight) / 2;
    for (int i = 0; i < numLines; ++i) {
        int lineWidth = MeasureText(lines[i].c_str(), fontSize);
        float x = messageRect.x + (popupWidth - lineWidth) / 2;
        float y = startY + i * fontSize * lineSpacing;
        DrawText(lines[i].c_str(), x, y, fontSize, BLACK);
    }
}

void PopUpWindow::DrawBuySellPopup(bool isBuyMode, bool& isVisible, Company* company, PlayerData& player)
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
    float inputValue = std::atof(inputBuffer);
    float sharesToBuy = inputValue / pricePerShare;
    float estimatedValue = inputValue * pricePerShare;

    int y = popup.y + 20;
    const float left = popup.x + 20;

    // Title
    DrawText(isBuyMode ? "ENTER AMOUNT TO INVEST" : "ENTER NUMBER OF SHARES TO SELL", left, y, 24, BLACK);
    y += 40;

    // Draw input box right after title
    Rectangle inputBounds = { popup.x + 20, (float)y, 200, 30 };

    // Draw the input box
    GuiSetStyle(TEXTBOX, BASE_COLOR_NORMAL, ColorToInt(WHITE));
    GuiSetStyle(TEXTBOX, BORDER_COLOR_NORMAL, ColorToInt(GRAY));
    GuiSetStyle(TEXTBOX, TEXT_COLOR_NORMAL, ColorToInt(BLACK));
    GuiTextBox(inputBounds, inputBuffer, sizeof(inputBuffer), isInputFocused);

    // Handle input focus/clicks
    if (CheckCollisionPointRec(GetMousePosition(), inputBounds)) {
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            isInputFocused = true;
            if (strcmp(inputBuffer, "0.0") == 0) strcpy(inputBuffer, "");
        }
    } else if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        isInputFocused = false;
        if (strlen(inputBuffer) == 0) strcpy(inputBuffer, "0.0");
    }

    y += 40; // Space after textbox

    // --- Shared Info ---
    DrawText(TextFormat("Price per Share: $%.2f", pricePerShare), left, y, 20, DARKGRAY);
    y += 30;

    DrawText(TextFormat("Current Shares Owned: %.2f", ownedShares), left, y, 20, DARKGRAY);
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
            }
            else if (TransactionManager::Instance().HasPendingBuy(stock)) {
                Show("You already have a buy order for this stock.");
            }
            else {
                TransactionManager::Instance().CreateBuyOrder(stock, PlayerData::Instance().weekExecutionDelay, inputValue);
                Show("Stock purchase placed! Will execute after delay.");
                isVisible = false;
                strcpy(inputBuffer, "");
                GameState::Instance().SetTempPause(false);
            }
        } else {
            if (!canSell) {
                if (inputValue < 0.01f) Show("Enter a valid share amount.");
                else if (inputValue > ownedShares) Show("You don't own that many shares.");
                else Show("Selling would drop you below the minimum shares.");
            }
            else if (TransactionManager::Instance().HasPendingSell(stock)) {
                Show("You already have a sell order for this stock.");
            }
            else {
                TransactionManager::Instance().CreateSellOrder(stock, PlayerData::Instance().weekExecutionDelay, inputValue);
                Show("Sell order placed! Will execute after delay.");
                isVisible = false;
                strcpy(inputBuffer, "");
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
        strcpy(inputBuffer, "");
        GameState::Instance().SetTempPause(false);
    }
}
