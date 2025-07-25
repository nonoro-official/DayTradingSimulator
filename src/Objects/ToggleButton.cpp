#include "ToggleButton.h"

ToggleButton::ToggleButton(Texture2D texture, Vector2 center, Vector2 bounds,
                           Rectangle sourceRect, float drawSize,
                           Color normalColor, Color hoverColor, Color toggleColor)
    : texture(texture), center(center), bounds(bounds),
      sourceRect(sourceRect), drawSize(drawSize),
      normalColor(normalColor), hoverColor(hoverColor), toggleColor(toggleColor)
{}

Rectangle ToggleButton::GetButtonRect() const {
    return {
        center.x - bounds.x * 0.5f,
        center.y - bounds.y * 0.5f,
        bounds.x,
        bounds.y
    };
}

void ToggleButton::SetToggled(bool toggled) {
    isToggled = toggled;
}


void ToggleButton::SetOnClick(std::function<void(bool)> callback) {
    onClickCallback = callback;
}

void ToggleButton::Update() {
    Rectangle btnRect = GetButtonRect();
    isHovered = CheckCollisionPointRec(GetMousePosition(), btnRect);

    if (isHovered && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        isToggled = !isToggled;
        if (onClickCallback) onClickCallback(isToggled);
    }
}

void ToggleButton::Draw() {
    Color tint = normalColor;
    if (isToggled) {
        tint = toggleColor;
    } else if (isHovered) {
        tint = hoverColor;
    }

    DrawTexturePro(
        texture,
        sourceRect,
        { center.x - bounds.x / 2, center.y - bounds.y / 2, bounds.x, bounds.y },
        { 0, 0 },
        0.0f,
        tint
    );
}
