//
// Created by sonik on 7/21/2025.
//

#include "Graph.h"
#include "raylib.h"
#include <cstdio>

char infoBuffer[64];

GraphPoint::GraphPoint(float x, float y) {
    position.x = x;
    position.y = y;
}

void GraphPoint::Draw() {
    if (nextPoint != nullptr) {
        DrawLineEx(position, nextPoint->position, 2.5, lineColor);
    }

    DrawCircle(position.x, position.y, radius, pointColor);

    if (CheckCollisionPointCircle(GetMousePosition(), position, radius)) {
        Vector2 boxPosition = {position.x - width / 2, position.y + yOffset + height / 2};
        Vector2 linePosition = {position.x, position.y + yOffset + height / 2};
        DrawLineEx(position, linePosition, 2.5, lineColor);
        DrawRectangle(boxPosition.x, boxPosition.y, width, height, boxColor);

        snprintf(infoBuffer, sizeof(infoBuffer), "Value: %.2f\nDifference: %.2f", 100.0f, -5.0f);
        DrawText(infoBuffer, boxPosition.x, boxPosition.y, fontSize, textColor);
    }
}

void GraphDisplay::Update() {
    for (GraphNode* node : nodes) {
        GraphPoint* point = node->point;
        point->position.x += scrollSpeed;
    }
}

void GraphDisplay::Draw() {
    for (GraphNode* node : nodes) {
        GraphPoint* point = node->point;
        point->Draw();
    }
}

void GraphDisplay::AddNode(GraphPoint* point) {
    GraphNode* lastNode = nodes.empty() ? nullptr : nodes.back();
    GraphNode* node = new GraphNode{point, lastNode};
    nodes.push_back(node);
}
