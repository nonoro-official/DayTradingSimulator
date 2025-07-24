//
// Created by sonik on 7/21/2025.
//

#include "Graph.h"

#include <algorithm>

#include "raylib.h"
#include <cstdio>
#include <iostream>

char infoBuffer[64];

GraphPoint::GraphPoint() {}

GraphPoint::GraphPoint(float x, float y) {
    position.x = x;
    position.y = y;
}

void GraphPoint::Draw() {
    if (prevPoint != nullptr) {
        DrawLineEx(position, prevPoint->position, 2.5f, lineColor);
    }

    DrawCircle(position.x, position.y, radius, pointColor);

    if (CheckCollisionPointCircle(GetMousePosition(), position, collisionRadius)) {
        const float padding = 8.0f;

        Vector2 boxPosition = {position.x - width / 2, position.y + yOffset + height / 2};
        Vector2 linePosition = {position.x, boxPosition.y};

        // Clamp box to screen bounds with padding
        int screenWidth = GetScreenWidth();
        int screenHeight = GetScreenHeight();

        if (boxPosition.x < padding) boxPosition.x = padding;
        if (boxPosition.x + width > screenWidth - padding)
            boxPosition.x = screenWidth - width - padding;

        if (boxPosition.y < padding) boxPosition.y = padding;
        if (boxPosition.y + height > screenHeight - padding)
            boxPosition.y = screenHeight - height - padding;

        DrawLineEx(position, linePosition, 2.5f, lineColor);
        DrawRectangle(boxPosition.x, boxPosition.y, width, height, boxColor);

        snprintf(infoBuffer, sizeof(infoBuffer), "Value: %.2f\nDifference: %.2f", 100.0f, -5.0f);
        DrawText(infoBuffer, boxPosition.x, boxPosition.y, fontSize, textColor);
    }
}


void GraphDisplay::Update() {
    timer += GetFrameTime();
    if (timer >= interval * (1 / GameState::Instance().GetTimeScale())) {
        timer = 0.0f;

        // Move points and mark those out of bounds
        for (int i = 0; i < nodes.size(); /* manual increment */) {
            GraphPoint* point = nodes[i];

            point->position.x += pixelsPerInterval;

            if (point->position.x < center.x - bounds.x / 2) {
                // Delete and remove node
                delete point;                     // Free heap memory
                nodes.erase(nodes.begin() + i); // Remove from vector
                continue; // Do not increment i, since the vector shifted
            }

            i++; // Only increment if we didn't erase
        }

        // Spawn new node if any in queue
        if (!queue.empty()) {
            ForceAddNode(queue.front());
            queue.erase(queue.begin());
        }
    }
}

void GraphDisplay::Draw() {
    // Draw background of the graph area
    DrawRectangle(center.x - bounds.x / 2, center.y - bounds.y / 2, bounds.x, bounds.y, displayColor);

    // === Draw Grid Lines ===
    float rightBorder = center.x - bounds.x / 2;
    float top = center.y - bounds.y / 2;
    float bottom = center.y + bounds.y / 2;

    for (int i = 0; i < pointsToDraw; i++) {
        float x = rightBorder - 5 - pixelsPerInterval * (i + 1);
        DrawLine((int)x, (int)top, (int)x, (int)bottom, gridColor);
    }

    // Draw all graph points and connections
    for (GraphPoint* node : nodes) {;
        node->Draw();
    }

    // Draw the outline of the graph area
    DrawRectangleLinesEx(
        { center.x - bounds.x / 2, center.y - bounds.y / 2, bounds.x, bounds.y },
        borderWidth,  // Thickness of the lines
        borderColor // Border color
    );
}

void GraphDisplay::AddNode(GraphPoint* point) {
    queue.push_back(point);
}

void GraphDisplay::ForceAddNode(GraphPoint* point) {
    if (!nodes.empty()) { point->prevPoint = nodes.back(); }
    nodes.push_back(point);

    // Assume value range (Perlin with amplitude 1 means 0–1 range)
    float dataMinY = 0.0f;
    float dataMaxY = 1.0f;

    float rawY = point->position.y;
    float normalizedY = (rawY - dataMinY) / (dataMaxY - dataMinY);
    normalizedY = std::clamp(normalizedY, 0.0f, 1.0f);

    // Flip so high value goes UP (screen Y grows downward)
    float top = center.y - bounds.y / 2.0f;
    float mappedY = top + (1.0f - normalizedY) * bounds.y;

    Vector2 position;
    position.x = center.x + bounds.x / 2.0f - 5;
    position.y = mappedY;

    // Assign prevPoint
    point->position = position;
}


GraphDisplay::GraphDisplay(Vector2 c, Vector2 b) {
    center = c;
    bounds = b;

    // calculate ppi
    pixelsPerInterval = ((center.x - bounds.x / 2) - (center.x + bounds.x / 2)) / pointsToDraw;

    // Pre-fill with flat data
    float rightBorder = center.x - bounds.x / 2;
    for (int i = 0; i < pointsToDraw; ++i) {
        GraphPoint* point = new GraphPoint();

        // Link to previous node if any
        if (!nodes.empty()) { point->prevPoint = nodes.back(); }
        nodes.push_back(point);

        // Normalized Y → screen Y mapping (same as ForceAddNode)
        float x = rightBorder - 5 - pixelsPerInterval * (i + 1);

        point->position.x = x;
        point->position.y = center.y;
    }
}

void GraphDisplay::AddPointsFromVector(const std::vector<GraphPoint>& points) {
    for (const GraphPoint& point : points) {
        AddNode(const_cast<GraphPoint*>(&point)); // safe only if original vector won't be moved
    }
}

GraphDisplay::~GraphDisplay() {
    for (GraphPoint *node : nodes) {
        delete node;
    }

    for (GraphPoint* node : queue) {
        delete node;
    }

    nodes.clear();
    queue.clear();
}