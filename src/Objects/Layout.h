//
// Created by Noah Peñaranda on 7/29/2025.
//

#ifndef LAYOUT_H
#define LAYOUT_H

struct Layout {
    float screenWidth;
    float screenHeight;

    float leftOffset = 120.0f;
    float topOffset = 60.0f;     // Matches top bar height
    float headerHeight = 50.0f;
    float rowHeight = 60.0f;
    float spacing = 8.0f;
    float sectionHeight = 60.0f; // height of top/bottom dashboard bars

    float GetBoxX() const { return leftOffset + 10.0f; }
    float GetBoxStartY() const { return topOffset + headerHeight + spacing; }
    float GetBoxWidth() const { return screenWidth - GetBoxX() - 10.0f; }

    Layout(float width, float height) : screenWidth(width), screenHeight(height) {}
};

#endif //LAYOUT_H
