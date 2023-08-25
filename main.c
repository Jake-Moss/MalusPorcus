#include "raylib.h"
// #include "raymath.h"
#include "stdio.h"

#define PHYSAC_IMPLEMENTATION
#include "physac.h"

#include "widget.c"

int main(void) {
    const int screenWidth = 640;
    const int screenHeight = 480;

    SetConfigFlags(FLAG_MSAA_4X_HINT);
    InitWindow(screenWidth, screenHeight, "Cozy Desk Simulator");

    // TODO: force aspect ratio

    // SetWindowOpacity(0.8); 

    SetTargetFPS(60);

    SetWindowState(FLAG_WINDOW_RESIZABLE);

    RenderTexture2D renderTexture = LoadRenderTexture(screenWidth, screenHeight);


    InitPhysics();
    SetPhysicsTimeStep(0.1); // <- default is 1.6 ms. Framerate is approx one 
                             // frame every 16ms, so were getting 10 steps (ish)
                             // per frame. Now like 160.

     // floor rectangle physics body
    PhysicsBody floor = CreatePhysicsBodyRectangle((Vector2){ screenWidth/2, screenHeight }, screenWidth, 100, 10);
    floor->enabled = false; // Disable body state to convert it to static (no dynamics, but collisions)

    while (!WindowShouldClose()) {

        // spawn some R E C T A N G L E S
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            // PhysicsBody rect = CreatePhysicsBodyRectangle(GetMousePosition(), GetRandomValue(100, 300),GetRandomValue(100, 200), 1.0);
            // rect->freezeOrient = true;

            // create a widget
            SimpleWidget myWidget = newWidget(GetMousePosition(), (Vector2){GetRandomValue(100, 200),GetRandomValue(100, 200)}, 3);
        }

        // destroy physics bodies that have fallen off screen
        int bodiesCount = GetPhysicsBodiesCount();
        for (int i = bodiesCount - 1; i >= 0; i--)
        {
            PhysicsBody body = GetPhysicsBody(i);
            
            if ((body != 0) && (body->position.y > screenHeight*2))
                DestroyPhysicsBody(body);
        }

        // typically things are done between `BeginDrawing()` and `EndDrawing()`.
        // Instead, we're going to draw to a texture, so our actual drawing is just
        // drawing the texture itself. Allows for scaling for pixel-accurate
        // resizing/fullscreen.
        BeginTextureMode(renderTexture);
            ClearBackground(RAYWHITE);

            // drawWidget(&myWidget);

            DrawText("WELCOME", screenWidth / 2, screenHeight / 2, 20, LIGHTGRAY);
            DrawText("For the dawgs", screenWidth / 2, screenHeight / 2 - 50, 20, LIGHTGRAY);
            
            // debug draw physics bodies
            bodiesCount = GetPhysicsBodiesCount();
            for (int i = 0; i < bodiesCount; i++)
            {
                PhysicsBody body = GetPhysicsBody(i);

                if (body != 0)
                {
                    int vertexCount = GetPhysicsShapeVerticesCount(i);
                    for (int j = 0; j < vertexCount; j++)
                    {
                        // Get physics bodies shape vertices to draw lines
                        // Note: GetPhysicsShapeVertex() already calculates rotation transformations
                        Vector2 vertexA = GetPhysicsShapeVertex(body, j);

                        int jj = (((j + 1) < vertexCount) ? (j + 1) : 0);   // Get next vertex or first to close the shape
                        Vector2 vertexB = GetPhysicsShapeVertex(body, jj);

                        DrawLineV(vertexA, vertexB, GREEN);     // Draw a line between two vertex positions
                    }
                }
            }
            
            
            DrawFPS(screenWidth - 90, screenHeight - 30);
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

    ClosePhysics();
    UnloadRenderTexture(renderTexture);
    CloseWindow();

    return 0;
}