//
// Created by sonik on 7/25/2025.
//

#include "MonthDisplay.h"

#include <iostream>
#include <string.h>
#include <string>

#include "../Classes/GameState.h"

void MonthDisplay::Update() {

    // Global Controls
    if (IsKeyPressed(KEY_P)) {
        GameState::Instance().PauseGame();

        // Sync button states
        buttons[0].SetToggled(GameState::Instance().IsPaused());
        buttons[1].SetToggled(!GameState::Instance().IsPaused());
        buttons[2].SetToggled(false);
        buttons[3].SetToggled(false);
    }

    if (IsKeyPressed(KEY_ONE)) {
        GameState::Instance().SetTimeScale(1.0f);

        // Reset speed buttons
        buttons[2].SetToggled(false);
        buttons[3].SetToggled(false);

        buttons[0].SetToggled(GameState::Instance().IsPaused());
        buttons[1].SetToggled(true);
    }

    if (IsKeyPressed(KEY_TWO)) {
        GameState::Instance().SetTimeScale(2.0f);

        // Update button visuals
        buttons[2].SetToggled(true);
        buttons[3].SetToggled(false);

        buttons[0].SetToggled(GameState::Instance().IsPaused());
        buttons[1].SetToggled(false);

    }

    if (IsKeyPressed(KEY_THREE)) {
        GameState::Instance().SetTimeScale(4.0f);

        buttons[2].SetToggled(false);
        buttons[3].SetToggled(true);

        buttons[0].SetToggled(GameState::Instance().IsPaused());
        buttons[1].SetToggled(false);

    }

    for (ToggleButton& button : buttons) {
        button.Update();
    }
}



void MonthDisplay::Draw() {
    float padding = 8.0f;

    // === Build single label string ===
    std::string label = "Month " + std::to_string(GameState::Instance().GetMonth()) +
                        " | Week " + std::to_string(GameState::Instance().GetWeek());

    // Draw background
    DrawRectangleV({center.x - bounds.x / 2, center.y - bounds.y / 2}, bounds, backgroundColor);

    // Draw border
    DrawRectangleLinesEx({center.x - bounds.x / 2, center.y - bounds.y / 2, bounds.x, bounds.y}, borderThickness, borderColor);

    // === Center the single combined label ===
    float textSize = MeasureText(label.c_str(), fontSize);

    Vector2 textPos = {
        center.x - textSize / 2 + padding,
        center.y - bounds.y / 2 + padding
    };

    DrawTextEx(GetFontDefault(), label.c_str(), textPos, fontSize, 1, textColor);

    // === Draw Buttons ===
    for (ToggleButton& button : buttons) {
        button.Draw();
    }
}



// Constructor
MonthDisplay::MonthDisplay(float fontSize, Vector2 center, Vector2 bounds,
                           float borderThickness, Color textColor,
                           Color backgroundColor, Color borderColor)
    : fontSize(fontSize), center(center), bounds(bounds),
      borderThickness(borderThickness), textColor(textColor),
      backgroundColor(backgroundColor), borderColor(borderColor)
{
    // === Layout Constants ===
    const float spacing = 2.0f;
    const float buttonSize = 20.0f;
    const float bottomOffset = 12.0f;

    // === Load Texture ===
    texture = LoadTexture("../Textures/uiTime.png");

    // === Compute Starting Button Position ===
    float totalWidth = (buttonSize + spacing) * 4 - spacing;
    Vector2 startPos = {
        center.x - bounds.x / 4,
        center.y - buttonSize / 2 + bottomOffset
    };

    // === Create Buttons ===
    for (int i = 0; i < 4; ++i) {
        // Center each button
        Vector2 buttonCenter = {
            startPos.x + i * (buttonSize + spacing) + buttonSize / 2 + 8,
            startPos.y + buttonSize / 2
        };

        // Sprite source from 2x2 grid in 16x16 tiles
        Rectangle spriteSource = {
            static_cast<float>((i % 2) * 16),
            static_cast<float>((i / 2) * 16),
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

    // === Assign Click Handlers ===
    for (int i = 0; i < 4; ++i) {
        buttons[i].SetOnClick([this, i](bool state) {
            if (state) {
                // Deselect all other buttons
                for (int j = 0; j < buttons.size(); ++j) {
                    if (j != i) buttons[j].SetToggled(false);
                }

                // Apply button functionality
                switch (i) {
                    case 0: GameState::Instance().SetPause(true);  break; // Pause
                    case 1: GameState::Instance().SetPause(false); break; // Play
                    case 2: GameState::Instance().SetTimeScale(2); break; // 2x Speed
                    case 3: GameState::Instance().SetTimeScale(4); break; // 4x Speed
                }

                TraceLog(LOG_INFO, "Button %d toggled ON", i);
            } else {
                // Ensure at least one button stays active
                buttons[i].SetToggled(true);
            }
        });
    }

    // === Set Default State ===
    buttons[1].SetToggled(true);  // Play by default
    GameState::Instance().SetPause(false);
}
