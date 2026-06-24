//
// Created by sonik on 7/25/2025.
//

#ifndef MONTHDISPLAY_H
#define MONTHDISPLAY_H

#include "raylib.h"
#include "ToggleButton.h"

class MonthDisplay {
public:
    void Draw();
    void Update();

    MonthDisplay(float fontSize, Vector2 center, Vector2 bounds, float borderThickness, Color textColor, Color backgroundColor, Color borderColor);

private:
    float fontSize;
    Vector2 center, bounds;
    float borderThickness;
    Color textColor, backgroundColor, borderColor;

    Texture2D texture;
    std::vector<ToggleButton> buttons;
};

#endif //MONTHDISPLAY_H
