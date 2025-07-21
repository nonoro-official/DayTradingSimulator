//
// Created by sonik on 7/21/2025.
//

#pragma once
#include "raylib.h"
#include <vector>

extern char infoBuffer[64];

class GraphPoint {
public:
    Vector2 position = {0, 0};
    GraphPoint* nextPoint = nullptr;
    Color pointColor = DARKGREEN, lineColor = DARKGREEN;

    GraphPoint(float x, float y);
    void Draw();

private:
    float radius = 5;
    float yOffset = -100;
    float width = 80, height = 40;
    float fontSize = 8;
    Color textColor = BLACK, boxColor = SKYBLUE;
};

class GraphDisplay {
    struct GraphNode {
        GraphPoint* point;
        GraphNode* next;
    };

public:
    Vector2 center;
    Vector2 bounds;
    void Update();
    void Draw();
    void AddNode(GraphPoint* point);
    void ForceAddNode(GraphPoint* point);

    GraphDisplay(Vector2 center, Vector2 bounds);

private:
    std::vector<GraphNode*> nodes;
    std::vector<GraphPoint*> queue;
    float pixelsPerInterval = -50;
    float timer = 0.0f;
    float interval = 1.0f; // time between adding nodes
};
