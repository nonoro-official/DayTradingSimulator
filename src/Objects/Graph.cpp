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
    DrawRectangle(center.x - bounds.x / 2, center.y - bounds.y / 2, bounds.x, bounds.y, GRAY);

    for (GraphNode* node : nodes) {
        GraphPoint* point = node->point;
        point->Draw();
    }
}

void GraphDisplay::AddNode(GraphPoint* point) {
    queue.push_back(point);
}

void GraphDisplay::ForceAddNode(GraphPoint* point) {
    GraphNode* lastNode = nodes.empty() ? nullptr : nodes.back();
    GraphNode* node = new GraphNode{point, lastNode};
    nodes.push_back(node);

    // initialize spawn
    Vector2 position = point->position;
    position.x = center.x + bounds.x / 2;
    node->point->position = position;
}

GraphDisplay::GraphDisplay(Vector2 c, Vector2 b) {
    center = c;
    bounds = b;
}
