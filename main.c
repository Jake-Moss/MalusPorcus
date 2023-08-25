#include <raylib.h>

int main(void) {
    const int screenWidth = 640;
    const int screenHeight = 480;

    InitWindow(screenWidth, screenHeight, "Cozy Desk Simulator");

    while (!WindowShouldClose()) {
        BeginDrawing();
            ClearBackground(RAYWHITE);

            DrawText("WELCOME", screenWidth / 2, screenHeight / 2, 20, LIGHTGRAY);
        EndDrawing();
    }

    CloseWindow();

    return 0;
}