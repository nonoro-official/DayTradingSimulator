#include "raylib.h"
#include <iostream>
#include <string>

#include "Objects/UI/Button.h"

const int screenWidth = 640;
const int screenHeight = 480;

class Player {
    public:
        Vector2 position;
        int radius;
        int moveSpeed;

        void Draw() {
            DrawCircle(position.x, position.y, radius, PURPLE);
        }

        void Update(Vector2 input) {
            position.x += input.x * moveSpeed;
            position.y += input.y * moveSpeed;

            if (position.x > screenWidth - radius) CloseWindow();
            if (position.x < radius) CloseWindow();
            if (position.y > screenHeight - radius) CloseWindow();
            if (position.y < radius) CloseWindow();
        }
};

int main()
{
    InitWindow(screenWidth, screenHeight, "Pong");

    SetTargetFPS(60);

    Player player = {{screenWidth / 2, screenHeight / 2}, 25, 5};

    Button test = {screenWidth / 2, screenHeight / 2, 30, 15};
    test.onClick = []() {
        std::cout << "Button clicked" << std::endl;
    };

    test.onHold = []() {
        std::cout << "Button held" << std::endl;
    };

    test.onRelease= []() {
        std::cout << "Button rel" << std::endl;
    };

    while (!WindowShouldClose())
    {
        // UPDATE
        Vector2 input = {0, 0};
        if (IsKeyDown(KEY_A)) input.x = -1;
        if (IsKeyDown(KEY_D)) input.x = 1;
        if (IsKeyDown(KEY_W)) input.y = -1;
        if (IsKeyDown(KEY_S)) input.y = 1;

        std::string inputStr = "INPUT: (" + std::to_string(input.x) + ", " + std::to_string(input.y) + ")";

        player.Update(input);
        test.Update();

        // DRAW
        BeginDrawing();
        ClearBackground(WHITE);

        DrawText(inputStr.c_str(), 0, 0, 20, BLACK);


        player.Draw();

        test.Draw();

        EndDrawing();
    }

    CloseWindow();

    return 0;
}
