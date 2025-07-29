//
// Created by Noah Peñaranda on 7/29/2025.
//

#include "MessageDisplay.h"
#include "raylib.h"

void MessageDisplay::Show(const std::string& msg)
{
    message = msg;
    isVisible = true;
}

void MessageDisplay::Hide()
{
    isVisible = false;
}

void MessageDisplay::Draw()
{
    if (isVisible)
    {
        Rectangle messageRect = { GetScreenWidth() / 2 - 150.0f, GetScreenHeight() / 2 - 50.0f, 300, 100 };
        DrawRectangleRec(messageRect, LIGHTGRAY);
        DrawRectangleLinesEx(messageRect, 1, DARKGRAY);

        int textWidth = MeasureText(message.c_str(), 20);
        DrawText(message.c_str(), GetScreenWidth() / 2 - textWidth / 2, GetScreenHeight() / 2 - 10, 20, BLACK);

        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
        {
            Hide();
        }
    }
}