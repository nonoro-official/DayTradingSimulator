#pragma once
#include "raylib.h"
#include <functional>

class Button {
public:
    Rectangle bounds;
    Color colorNormal = GRAY;
    Color colorHover = DARKGRAY;
    Color colorPressed = LIGHTGRAY;

    std::function<void()> onClick = nullptr;
    std::function<void()> onHold = nullptr;
    std::function<void()> onRelease = nullptr;

    Button(float x, float y, float w, float h);

    void Update();
    void Draw();
private:
    bool isHovered = false;
    bool wasPressed = false;
};
