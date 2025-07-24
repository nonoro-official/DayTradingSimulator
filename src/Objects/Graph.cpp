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
    timer += GetFrameTime();
    if (timer >= interval) {
        timer = 0.0f;

        // Move points and mark those out of bounds
        for (int i = 0; i < nodes.size(); /* manual increment */) {
            GraphNode* node = nodes[i];
            GraphPoint* point = node->point;

            point->position.x += pixelsPerInterval;

            if (point->position.x < center.x - bounds.x / 2) {
                // Delete and remove node
                delete node;                     // Free heap memory
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
    DrawRectangle(center.x - bounds.x / 2, center.y - bounds.y / 2, bounds.x, bounds.y, GRAY);

    // Draw all graph points and connections
    for (GraphNode* node : nodes) {
        GraphPoint* point = node->point;
        point->Draw();
    }

    // Draw the outline of the graph area
    DrawRectangleLinesEx(
        { center.x - bounds.x / 2, center.y - bounds.y / 2, bounds.x, bounds.y },
        2.0f,  // Thickness of the lines
        DARKGRAY // Border color
    );
}

void GraphDisplay::AddNode(GraphPoint* point) {
    queue.push_back(point);
}

void GraphDisplay::ForceAddNode(GraphPoint* point) {
    GraphNode* lastNode = nodes.empty() ? nullptr : nodes.back();
    GraphNode* node = new GraphNode{point, lastNode};
    nodes.push_back(node);

    // Assume value range (Perlin with amplitude 1 means 0–1 range)
    float dataMinY = 0.0f;
    float dataMaxY = 1.0f;

    float rawY = point->position.y;
    float normalizedY = (rawY - dataMinY) / (dataMaxY - dataMinY);
    normalizedY = std::clamp(normalizedY, 0.0f, 1.0f);

    // Flip so high value goes UP (screen Y grows downward)
    float top = center.y - bounds.y / 2.0f;
    float mappedY = top + (1.0f - normalizedY) * bounds.y;

    // DEBUG
    std::cout << normalizedY << std::endl;

    Vector2 position;
    position.x = center.x + bounds.x / 2.0f - 5;
    position.y = mappedY;

    node->point->position = position;
}


GraphDisplay::GraphDisplay(Vector2 c, Vector2 b) {
    center = c;
    bounds = b;

    // calculate ppi
    pixelsPerInterval = ((center.x - bounds.x / 2) - (center.x + bounds.x / 2)) / pointsToDraw;
}

void GraphDisplay::AddNodesFromVector(const std::vector<GraphPoint>& points) {
    for (const GraphPoint& point : points) {
        AddNode(const_cast<GraphPoint*>(&point)); // safe only if original vector won't be moved
    }
}

GraphDisplay::~GraphDisplay() {
    for (GraphNode* node : nodes) {
        delete node;
    }

    // If `queue` contains dynamically allocated GraphPoint*, and
    // if you're not deleting them elsewhere, also clean them up:
    for (GraphPoint* point : queue) {
        delete point;
    }

    nodes.clear();
    queue.clear();
}