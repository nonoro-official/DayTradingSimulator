//
// Created by sonik on 7/21/2025.
//

#include "Graph.h"

#include <algorithm>

#include "raylib.h"
#include <cstdio>
#include <iostream>

#include "raymath.h"

char infoBuffer[64];

GraphPoint::GraphPoint(float y) {
    position.x = 0;
    position.y = 0;
    yValue = y;
}

void GraphPoint::InitializeTooltip(int week, int month, float price, float increase) {
    this->week = week;
    this->month = month;
    this->price = price;
    this->increase = increase;
}


void GraphPoint::Update() {
    hovering = CheckCollisionPointCircle(GetMousePosition(), position, collisionRadius);
}

void GraphPoint::DrawLineToPrevious() {
    if (prevPoint != nullptr) {
        DrawLineEx(position, prevPoint->position, 2.5f, lineColor);
    }
}

void GraphPoint::DrawPoint() {
    DrawCircleV(position, radius, pointColor);
}

void GraphPoint::DrawTooltip(Vector2 graphCenter, Vector2 graphBounds) {
    if (!hovering) return;

    const float padding = 8.0f;

    Vector2 boxPosition = {
        position.x - width / 2,
        position.y + yOffset + height / 2
    };

    // Clamp within bounds
    float left   = graphCenter.x - graphBounds.x / 2.0f + padding;
    float right  = graphCenter.x + graphBounds.x / 2.0f - padding - width;
    float top    = graphCenter.y - graphBounds.y / 2.0f + padding;
    float bottom = graphCenter.y + graphBounds.y / 2.0f - padding - height;

    if (boxPosition.x < left) boxPosition.x = left;
    if (boxPosition.x > right) boxPosition.x = right;
    if (boxPosition.y < top) boxPosition.y = top;
    if (boxPosition.y > bottom) boxPosition.y = bottom;

    // Draw connector line to center of tooltip box
    Vector2 lineTarget = {
        boxPosition.x + width / 2.0f,
        boxPosition.y + height / 2.0f
    };

    DrawLineEx(position, lineTarget, 2.5f, lineColor);
    DrawRectangle(boxPosition.x, boxPosition.y, width, height, boxColor);

    // Placeholder values — replace with real data later
    snprintf(infoBuffer, sizeof(infoBuffer),
             "Month: %d\nWeek: %d\nPrice: %.2f\nChange: %.2f",
             month, week, price, increase);

    DrawText(infoBuffer, boxPosition.x + 6, boxPosition.y + 6, fontSize, textColor);
}

void GraphDisplay::Update() {
    if (!active) return;
    for (GraphPoint* node : nodes) {
        node->Update();
        if (node->hovering) {
            isAnyHovering = true;
        }
    }

    if (isAnyHovering == true) {
        hoverCooldownFrames = hoverGracePeriod;
        GameState::Instance().SetTempPause(true);
    } else {
        if (hoverCooldownFrames > 0) {
            hoverCooldownFrames--;
        } else {
            GameState::Instance().SetTempPause(false);
        }
    }
}



void GraphDisplay::DrawTooltips() {
    for (GraphPoint* node : nodes) {
        node->DrawTooltip(center, bounds); // Pass graph bounds
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
    for (GraphPoint* node : nodes) {
        node->DrawLineToPrevious(); // Pass graph bounds
    }

    for (GraphPoint* node : nodes) {
        node->DrawPoint(); // Pass graph bounds
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

    float rawY = point->yValue;

    // Flip so high value goes UP (screen Y grows downward)
    Vector2 position;
    position.x = center.x + bounds.x / 2.0f - 5;
    position.y = (center.y + bounds.y / 2) - rawY * bounds.y;

    // Assign prevPoint
    point->position = position;
}


GraphDisplay::GraphDisplay(Vector2 c, Vector2 b) {
    center = c;
    bounds = b;

    // calculate ppi
    pixelsPerInterval = ((center.x - bounds.x / 2) - (center.x + bounds.x / 2)) / pointsToDraw;

    GameState::Instance().AddTickListener([this]() {
        // OnTick callback logic here

        // Move points and mark those out of bounds
        for (int i = 0; i < nodes.size(); /* manual increment */) {
            GraphPoint* point = nodes[i];
            point->position.x += pixelsPerInterval;

            if (point->position.x < center.x - bounds.x / 2) {
                delete point;
                nodes.erase(nodes.begin() + i);
                continue;
            }
            i++;
        }

        // Add next queued point
        if (!queue.empty()) {
            ForceAddNode(queue.front());
            queue.erase(queue.begin());
        }
    });
}

void GraphDisplay::PrefillPoints(const std::vector<GraphPoint*>& points) {
    float rightBorder = center.x - bounds.x / 2;

    for (int i = 0; i < points.size(); ++i) {
        GraphPoint* point = points[i];

        // Recalculate position just like in ForceAddNode
        point->prevPoint = nodes.empty() ? nullptr : nodes.back();

        float rawY = point->yValue;
        Vector2 position;
        position.x = rightBorder - 5 - pixelsPerInterval * (i + 1);
        position.y = (center.y + bounds.y / 2) - rawY * bounds.y;
        point->position = position;

        nodes.push_back(point);
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