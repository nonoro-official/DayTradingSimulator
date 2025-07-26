//
// Created by sonik on 7/21/2025.
//

#pragma once
#include "raylib.h"
#include <vector>

#include "../Classes/GameState.h"

extern char infoBuffer[64];

class GraphPoint {
public:
    Vector2 position = {0, 0};
    GraphPoint* prevPoint = nullptr;
    Color pointColor = DARKGREEN, lineColor = Fade(DARKGREEN, .25f);

    GraphPoint();
    GraphPoint(float x, float y);
    void Draw(Vector2 graphCenter, Vector2 graphBounds);
    void Update();

private:
    float radius = 5.0f;
    float collisionRadius = 10.0f;
    float yOffset = -125;
    float width = 80, height = 40;
    float fontSize = 8;
    Color textColor = BLACK, boxColor = SKYBLUE;

    bool hovering;
};

class GraphDisplay {
public:
    int pointsToDraw = 25;

    Vector2 center;
    Vector2 bounds;
    void Update();
    void Draw();
    void AddNode(GraphPoint* point);
    void ForceAddNode(GraphPoint* point);

    GraphDisplay(Vector2 center, Vector2 bounds);
    void AddPointsFromVector(const std::vector<GraphPoint>& points);

    ~GraphDisplay();

private:
    std::vector<GraphPoint*> nodes;
    std::vector<GraphPoint*> queue;

    float pixelsPerInterval = 0; // calculated on create
    GameState* gameState = nullptr;

    Color gridColor = Fade(DARKGRAY, .25f);
    Color displayColor = Fade(BLACK, 0.9f);
    Color borderColor = BLACK;
    int borderWidth = 5;
};
