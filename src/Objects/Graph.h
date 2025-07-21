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

    GraphPoint(float x, float y);
    void Draw();

private:
    float radius = 5;
    Color pointColor = DARKGREEN, lineColor = DARKGREEN;
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
    void Update();
    void Draw();
    void AddNode(GraphPoint* point);

private:
    std::vector<GraphNode*> nodes;
    float scrollSpeed = -.25f;
};
