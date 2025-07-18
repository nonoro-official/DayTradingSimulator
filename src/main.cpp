#include "raylib.h"
#include <iostream>
#include <string>

int main()
{
    const int screenWidth = 640;
    const int screenHeight = 480;

    InitWindow(screenWidth, screenHeight, "Pong");

    SetTargetFPS(60);

    int radius = 25;
    int startPosX = screenWidth / 2;
    int startPosY = screenHeight / 2;
    Vector2 position = {0, 0};
    position.x = startPosX;
    position.y = startPosY;

    int moveSpeed = 5;

    while (!WindowShouldClose())
    {
        // UPDATE
        Vector2 input = {0, 0};
        if (IsKeyDown(KEY_A)) input.x = -1;
        if (IsKeyDown(KEY_D)) input.x = 1;
        if (IsKeyDown(KEY_W)) input.y = -1;
        if (IsKeyDown(KEY_S)) input.y = 1;

        position.x += input.x * moveSpeed;
        position.y += input.y * moveSpeed;

        // check if collide
        if (position.x > screenWidth - radius) CloseWindow();
        if (position.x < radius) CloseWindow();
        if (position.y > screenHeight - radius) CloseWindow();
        if (position.y < radius) CloseWindow();

        std::string inputStr = "INPUT: (" + std::to_string(input.x) + ", " + std::to_string(input.y) + ")";

        // DRAW
        BeginDrawing();
        ClearBackground(WHITE);

        DrawText(inputStr.c_str(), 0, 0, 20, BLACK);

        DrawCircle(position.x, position.y, radius, PURPLE);

        EndDrawing();
    }

    CloseWindow();

    return 0;
}
