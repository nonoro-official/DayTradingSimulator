//
// Created by Noah Peñaranda on 7/29/2025.
//

#include "PopUpWindow.h"
#include "raylib.h"



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