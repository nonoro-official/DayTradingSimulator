#include "raylib.h"
#include <iostream>
#include <string>

#include "Objects/UI/Button.h"

void Update();
void Draw();

class GraphPoint;
class GraphDisplay;

char infoBuffer[64];

class GraphDisplay {
struct GraphNode {
    GraphPoint* point;
    GraphNode* next;
};
public:
    GraphNode points;

    void Update() {

    }

    void Draw() {

    }

    void AddNode(GraphPoint* point) {
        // do recusrion

    }
private:
    float scrollSpeed = .25;
};

class GraphPoint {
public:
    Vector2 position = {0, 0};
    GraphPoint* nextPoint = nullptr;

    GraphPoint(float x, float y) {
        position.x = x;
        position.y = y;
    }

    void Draw() {
        if (nextPoint != nullptr) {
            DrawLineEx(position, nextPoint->position, 2.5, lineColor);
        }

        DrawCircle(position.x, position.y, radius, pointColor);

        // Draw info on hover
        if (CheckCollisionPointCircle(GetMousePosition(), position, radius)) {
            Vector2 boxPosition = {position.x - width / 2, position.y + yOffset + height / 2};
            Vector2 linePosition = {position.x, position.y + yOffset + height / 2};
            DrawLineEx(position, linePosition, 2.5, lineColor);
            DrawRectangle(boxPosition.x, boxPosition.y, width, height, boxColor);

            // Display info
            snprintf(infoBuffer, sizeof(infoBuffer), "Value: %.2f\nDifference: %.2f", 100, -5);
            char* infoText = infoBuffer;
            DrawText(infoText, boxPosition.x, boxPosition.y, fontSize, textColor);
        }
    }

private:
    float radius = 5;
    Color pointColor = DARKGREEN, lineColor = DARKGREEN;

    // Display box
    float yOffset = -100;
    float width = 80, height = 40;
    float fontSize = 8;
    Color textColor = BLACK, boxColor = SKYBLUE;
};

const int screenWidth = 640, screenHeight = 480;

int main()
{
    InitWindow(screenWidth, screenHeight, "Day Trading Simulator");

    SetTargetFPS(60);

    GraphPoint points[] = {
        { screenWidth / 2, screenHeight / 2 },
        {screenWidth / 2 - 15 * 2, screenHeight / 2 - 15},
        {screenWidth / 2 - 30 * 2, screenHeight / 2},
        {screenWidth / 2 - 45 * 2, screenHeight / 2 + 15}
    };

    // Connect points
    for (int i = 0; i < std::size(points); i++) {
        if (i < std::size(points)) {
            points[i].nextPoint = (points + 1);
        }
    }

    while (!WindowShouldClose())
    {
        // UPDATE


        // DRAW
        BeginDrawing();
        ClearBackground(WHITE);

        for (int i = 0; i < std::size(points); i++) {
            points[i].Draw();
            points[i].position.x -= 0;
        }

        EndDrawing();
    }

    CloseWindow();

    return 0;
}
