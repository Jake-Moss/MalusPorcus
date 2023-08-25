#include <raylib.h>

int main(void) {
    const int screenWidth = 640;
    const int screenHeight = 480;

    InitWindow(screenWidth, screenHeight, "Cozy Desk Simulator");

    // TODO: force aspect ratio

    // SetWindowOpacity(0.8); 

    SetTargetFPS(60);

    SetWindowState(FLAG_WINDOW_RESIZABLE);

    RenderTexture2D renderTexture = LoadRenderTexture(screenWidth, screenHeight);

    while (!WindowShouldClose()) {
        // typically things are done between `BeginDrawing()` and `EndDrawing()`.
        // Instead, we're going to draw to a texture, so our actual drawing is just
        // drawing the texture itself. Allows for scaling for pixel-accurate
        // resizing/fullscreen.
        BeginTextureMode(renderTexture);
            ClearBackground(RAYWHITE);

            DrawText("WELCOME", screenWidth / 2, screenHeight / 2, 20, LIGHTGRAY);
            DrawText("For the dawgs", screenWidth / 2, screenHeight / 2 - 50, 20, LIGHTGRAY);
        EndTextureMode();

        BeginDrawing(); // nobody touch this! Do your drawing in the BeginTextureMode above
            DrawTexturePro(
                renderTexture.texture,
                (Rectangle){ 0, 0, (float)(renderTexture.texture.width), (float)(-renderTexture.texture.height) },
                (Rectangle){ 0, 0, (float)(GetScreenWidth()), (float)(GetScreenHeight()) },
                (Vector2){ 0, 0 },
                0,
                WHITE
            );
        EndDrawing();
    }

    UnloadRenderTexture(renderTexture);

    CloseWindow();

    return 0;
}