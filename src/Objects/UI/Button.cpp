#include "Button.h"

Button::Button(float x, float y, float w, float h) {
    bounds = {x, y, w, h};
}

void Button::Update() {
    Vector2 mouse = GetMousePosition();
    isHovered = CheckCollisionPointRec(mouse, bounds);
    bool isMouseDown = IsMouseButtonDown(MOUSE_LEFT_BUTTON);
    bool isMousePressed = IsMouseButtonPressed(MOUSE_LEFT_BUTTON);
    bool isMouseReleased = IsMouseButtonReleased(MOUSE_LEFT_BUTTON);

    if (isHovered) {
        if (isMousePressed) {
            wasPressed = true;
            if (onClick) onClick();
        } else if (isMouseDown) {
            if (onHold) onHold();
        } else if (wasPressed && isMouseReleased) {
            wasPressed = false;
            if (onRelease) onRelease();
        }
    } else {
        wasPressed = false; // Reset if mouse leaves
    }
}

void Button::Draw() {
    Color currentColor = colorNormal;
    if (isHovered && IsMouseButtonDown(MOUSE_LEFT_BUTTON))
        currentColor = colorPressed;
    else if (isHovered)
        currentColor = colorHover;

    DrawRectangleRec(bounds, currentColor);
    DrawRectangleLinesEx(bounds, 2, BLACK);
}
