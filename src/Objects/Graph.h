//
// Created by sonik on 7/21/2025.
//

#pragma once
#include "raylib.h"
#include <vector>

#include "../Classes/GameState.h"
#include "Classes/MarketData.h"

extern char infoBuffer[64];

class GraphPoint {
public:
    float yValue;
    Vector2 position = {0, 0};
    GraphPoint* prevPoint = nullptr;

    Color pointColor = {0, 200, 83, 255};           // Emerald green
    Color lineColor  = Fade(pointColor, 0.25f);     // Light transparent line

    bool hovering = false;

    GraphPoint(float y);
    void InitializeTooltip(int week, int month, float price, float increase);
    void DrawLineToPrevious();
    void DrawPoint();
    void DrawTooltip(Vector2 graphCenter, Vector2 graphBounds);
    void Update();

private:
    float radius = 5.0f;
    float collisionRadius = 18.0f;

    float yOffset = -125;
    float width = 100, height = 60;
    float fontSize = 8;

    Color textColor = {34, 34, 34, 255};            // Near-black text
    Color boxColor  = {255, 255, 255, 230};         // Soft white tooltip box

    // Tooltip variables
    int week, month;
    float price, increase;
};
class GraphDisplay {
public:
    static bool isAnyHovering;
    int pointsToDraw = 24;

    Vector2 center;
    Vector2 bounds;
    void Update();
    void Draw();
    void DrawTooltips();
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

    int hoverCooldownFrames = 0;
    const int hoverGracePeriod = 24;
};