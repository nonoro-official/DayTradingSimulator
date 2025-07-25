//
// Created by sonik on 7/25/2025.
//

#include "MonthDisplay.h"

#include <iostream>
#include <string.h>
#include <string>

#include "GameState.h"

void MonthDisplay::Update() {
    for (ToggleButton& button : buttons) {
        button.Update();
    }
}


void MonthDisplay::Draw() {
    float padding = 8.0f;  // Change as needed

    std::string monthLabel = "Month " + std::to_string(GameState::Instance().GetMonth());
    std::string weekLabel = "Week " + std::to_string(GameState::Instance().GetWeek());

    // Draw background
    DrawRectangleV({center.x - bounds.x / 2, center.y - bounds.y / 2}, bounds, backgroundColor);

    // Draw border
    DrawRectangleLinesEx({center.x - bounds.x / 2, center.y - bounds.y / 2, bounds.x, bounds.y}, borderThickness, borderColor);

    // Top-left corner of the box
    Vector2 topLeft = {
        center.x - bounds.x / 2 + padding,
        center.y - bounds.y / 2 + padding
    };

    // Draw month label
    DrawTextEx(GetFontDefault(), monthLabel.c_str(), topLeft, fontSize, 1, textColor);

    // Draw week label below it
    Vector2 weekPos = {
        topLeft.x,
        topLeft.y + fontSize + 4  // 4 pixels spacing
    };
    DrawTextEx(GetFontDefault(), weekLabel.c_str(), weekPos, fontSize - 4, 1, textColor);

    /*Texture2D texture = LoadTexture("../Textures/uiTime.png");

    // Source rectangle (sprite to draw from the sheet)
    Rectangle source = { 0, 0, 16, 16 };  // x, y, width, height on the spritesheet

    // Destination rectangle (where and how big to draw on screen)
    Rectangle dest = { center.x, center.y, 24, 24 };  // x, y, width *scale*, height *scale*

    // Origin and rotation (optional)
    Vector2 origin = { 0, 0 };  // top-left corner
    float rotation = 0.0f;

    DrawTexturePro(texture, source, dest, origin, rotation, RED);*/

    for (ToggleButton& button : buttons) {
        button.Draw();
    }
}

#include "MonthDisplay.h"
#include "GameState.h"
#include <string>
// Constructor
MonthDisplay::MonthDisplay(float fontSize, Vector2 center, Vector2 bounds,
                           float borderThickness, Color textColor,
                           Color backgroundColor, Color borderColor)
    : fontSize(fontSize), center(center), bounds(bounds),
      borderThickness(borderThickness), textColor(textColor),
      backgroundColor(backgroundColor), borderColor(borderColor)
{
    float spacing = 4.0f;
    float buttonSize = 20.0f;

    texture = LoadTexture("../Textures/uiTime.png");

    Vector2 startPos = {
        center.x - ((buttonSize + spacing) * 2 - spacing) / 2,
        center.y + bounds.y / 2 - buttonSize - 8.0f
    };

    // create and store buttons
    for (int i = 0; i < 4; ++i) {
        Vector2 buttonCenter = {
            startPos.x + i * (buttonSize + spacing) + buttonSize / 2,
            startPos.y + buttonSize / 2
        };

        Rectangle spriteSource = {
            (float)((i % 2) * 16),
            (float)((i / 2) * 16),
            16, 16
        };

        ToggleButton button(
            texture,
            buttonCenter,
            { buttonSize, buttonSize },
            spriteSource,
            buttonSize,
            WHITE, GRAY, RED
        );

        buttons.push_back(button);
    }

    // assign click handlers
    for (int i = 0; i < 4; ++i) {
        buttons[i].SetOnClick([this, i](bool state) {
            if (state) {
                // Deselect others
                for (int j = 0; j < buttons.size(); ++j) {
                    if (j != i) buttons[j].SetToggled(false);
                }

                // Apply functionality
                switch (i) {
                    case 0: GameState::Instance().SetPause(true); break;
                    case 1: GameState::Instance().SetPause(false); break;
                    case 2: GameState::Instance().SetTimeScale(2); break;
                    case 3: GameState::Instance().SetTimeScale(4); break;
                }

                TraceLog(LOG_INFO, "Button %d toggled ON", i);
            } else {
                // Prevent all from being off
                buttons[i].SetToggled(true);
            }
        });
    }

    // Optional: Set default active button
    buttons[1].SetToggled(true);
    GameState::Instance().SetPause(false);
}
