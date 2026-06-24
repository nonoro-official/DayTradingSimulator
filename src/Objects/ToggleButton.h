#pragma once
#include "raylib.h"
#include <functional>

class ToggleButton {
public:
    ToggleButton(Texture2D texture, Vector2 center, Vector2 bounds,
             Rectangle sourceRect, float drawSize,
             Color normalColor, Color hoverColor, Color toggleColor);

    void Draw();
    void Update();
    void SetToggled(bool toggled);
    void SetOnClick(std::function<void(bool)> callback);  // Callback with toggle state

private:
    Rectangle sourceRect;
    Texture2D texture;
    Vector2 center;
    Vector2 bounds;
    float spriteSize;
    float drawSize;

    Color normalColor;
    Color hoverColor;
    Color toggleColor;

    bool isHovered = false;
    bool isToggled = false;

    std::function<void(bool)> onClickCallback;

    Rectangle GetButtonRect() const;
};
