#include "raylib.h"
#include "raymath.h"
#include "physac.h"

#include "stdlib.h"
#include <stdio.h>
#include "widget.h"
#include "helper.h"
#include "digital_clock.h"
#define PHYSAC_IMPLEMENTATION
#include "physac.h"


#define MAX(a, b) ((a)>(b)? (a) : (b))
#define MIN(a, b) ((a)<(b)? (a) : (b))

#define SCREEN_WIDTH 960
#define SCREEN_HEIGHT 720

int main(int argc, char* argv[]) {
    const int screenWidth = 960;
    const int screenHeight = 720;

    SetConfigFlags(FLAG_MSAA_4X_HINT);
    // SetConfigFlags(FLAG_WINDOW_UNDECORATED); // <-- kinda cool
    InitWindow(screenWidth, screenHeight, "Cozy Desk Simulator");

    // TODO: force aspect ratio

    // SetWindowOpacity(0.8); 

    SetTargetFPS(60);

    SetWindowState(FLAG_WINDOW_RESIZABLE);

    RenderTexture2D renderTexture = LoadRenderTexture(screenWidth, screenHeight);
    SetTextureFilter(renderTexture.texture, TEXTURE_FILTER_POINT);


    InitPhysics();
    SetPhysicsTimeStep(0.1); // <- default is 1.6 ms. Framerate is approx one 
                             // frame every 16ms, so were getting 10 steps (ish)
                             // per frame. Now like 160.

     // floor rectangle physics body
    int deskHeight = 100;
    PhysicsBody desk = CreatePhysicsBodyRectangle((Vector2){ screenWidth/2.0, screenHeight }, screenWidth, deskHeight, 10);

    desk->enabled = false; // Disable body state to convert it to static (no dynamics, but collisions)
    // Lets now add some pizazz to the "floor" (or really the desk)


    bool mouseClickHandled;
    bool somethingIsClickedOn = false;

    WidgetArray myWidgets;
    initWidgetArray(&myWidgets, 3);
    Font font = GetFontDefault();

    bool debugRender = false;


    bool isMenuVisible = false; // menu state
    Rectangle plusButton = {760, 20, 20, 20};
    Rectangle optionRects[1] = {
        {700, 50, 100, 30},
    };
    while (!WindowShouldClose()) {
        float scale = MIN((float)GetScreenWidth()/screenWidth, (float)GetScreenHeight()/screenHeight);

        Vector2 mouse = GetMousePosition();
        Vector2 virtualMouse = { 0 };
        virtualMouse.x = (mouse.x - (GetScreenWidth() - (screenWidth*scale))*0.5f)/scale;
        virtualMouse.y = (mouse.y - (GetScreenHeight() - (screenHeight*scale))*0.5f)/scale;
        virtualMouse = Vector2Clamp(virtualMouse, (Vector2){ 0, 0 }, (Vector2){ (float)screenWidth, (float)screenHeight });



        if (CheckCollisionPointRec(virtualMouse, plusButton) && IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) {
            isMenuVisible = !isMenuVisible;
        }


        // wrap when the menu is visible so that it doesn't draw things while clicking on the menu items
        // Check for click outside menu
        //

        // start of menu stuff
        if (isMenuVisible) {
            if (isMenuVisible && !CheckCollisionPointRec(virtualMouse, plusButton) && IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) {
                bool isOutside = true;
                for (int i = 0; i < 3; i++) {
                    if (CheckCollisionPointRec(mouse, optionRects[i])) {
                        isOutside = false;
                        break;
                    }
                }
                if (isOutside) {
                    isMenuVisible = false;
                }
            }


            typedef Widget (*f)(Vector2, Vector2, Font); // void function typedef
            f widgetOptions[1] = {&createDigitalClock};
            // Check for clicks on individual options in the menu
            if (isMenuVisible && IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) {
                for (int i = 0; i < 1; i++) {
                    if (CheckCollisionPointRec(mouse, optionRects[i])) {
                        Widget newWidget = widgetOptions[i]((Vector2){GetRandomValue(0,SCREEN_WIDTH), 0}, (Vector2){240, 100}, font);
                        insertWidgetArray(&myWidgets, newWidget);
                    }
                }
            }
        } // end of menu stuff
        // start of rest of functionality
        else {
            // we don't have a "mouse just clicked" thing, so we'll need to track state manually
            if (IsMouseButtonUp(MOUSE_LEFT_BUTTON)) {
                mouseClickHandled = false;
                somethingIsClickedOn = false;

                // reset grabbed state
                for (int i = 0; i < myWidgets.count; i++) {
                    myWidgets.array[i].isGrabbed = false;
                    myWidgets.array[i].grabOffset = (Vector2) {0.0, 0.0};
                }
            }

            if (IsKeyPressed(KEY_D)) debugRender = !debugRender;

            // now actually handle that click
            for (int i = 0; i < myWidgets.count; i++) {
                Widget testWidget = myWidgets.array[i];
                PhysicsBody testBody = testWidget.body;
                int vertexCount = testBody->shape.vertexData.vertexCount;


                somethingIsClickedOn = somethingIsClickedOn || pointInPhysicsBody(virtualMouse, testBody, vertexCount);
                if (pointInPhysicsBody(virtualMouse, testBody, vertexCount) && IsMouseButtonDown(MOUSE_BUTTON_LEFT) && !mouseClickHandled) { // this is a bad place to put mouseClickHandled, wastes some resources
                    mouseClickHandled = true;
                    myWidgets.array[i].isGrabbed = true;
                    myWidgets.array[i].grabOffset = Vector2Subtract(virtualMouse, myWidgets.array[i].body->position);
                }

                // if it's grabbed, or whatever else is grabbed, apply a force
                moveWhenGrabbed(&myWidgets.array[i], virtualMouse);
            }

            // no more clicking to spawn items...
            //// spawn some R E C T A N G L E S
            //if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && !somethingIsClickedOn) {
            //    // PhysicsBody rect = CreatePhysicsBodyRectangle(GetMousePosition(), GetRandomValue(100, 300),GetRandomValue(100, 200), 1.0);
            //    // rect->freezeOrient = true;

            //    // create a widget
            //    Widget aWidget = newWidget(virtualMouse, (Vector2){GetRandomValue(100, 200),GetRandomValue(100, 200)}, 3);
            //    insertWidgetArray(&myWidgets, aWidget);
            //} else if (IsMouseButtonPressed(MOUSE_RIGHT_BUTTON) && !somethingIsClickedOn) {
            //    //Widget bWidget = newWidget(virtualMouse, (Vector2){120, 80}, 3);
            //    Widget digitalClockWidget = createDigitalClock(virtualMouse, (Vector2){120, 80}, font);
            //    insertWidgetArray(&myWidgets, digitalClockWidget);
            //}

            // destroy physics bodies that have fallen off screen
            int bodiesCount = GetPhysicsBodiesCount();
            for (int i = bodiesCount - 1; i >= 0; i--)
            {
                PhysicsBody body = GetPhysicsBody(i);

                if ((body != 0) && (body->position.y > screenHeight*2))
                    DestroyPhysicsBody(body);
            }
        }
        // typically things are done between `BeginDrawing()` and `EndDrawing()`.
        // Instead, we're going to draw to a texture, so our actual drawing is just
        // drawing the texture itself. Allows for scaling for pixel-accurate
        // resizing/fullscreen.
        BeginTextureMode(renderTexture);
        ClearBackground(RAYWHITE);

        // drawWidget(&myWidget);

        //DrawText("WELCOME", screenWidth / 2, screenHeight / 2, 20, LIGHTGRAY);
        //DrawText("For the dawgs", screenWidth / 2, screenHeight / 2 - 50, 20, LIGHTGRAY);

        // draw and colour in the fg and bg
        //
        // getting the dimensions of the floor, then creating a rectangular shape
        Color deskColor = (Color){25, 25, 26, 255};
        DrawRectangle(0, SCREEN_HEIGHT-deskHeight/2, SCREEN_WIDTH, deskHeight/2, deskColor);

        DrawRectangleLines(plusButton.x, plusButton.y, plusButton.width, plusButton.height, BLACK);
        DrawLine(plusButton.x + 5, plusButton.y + 10, plusButton.x + 15, plusButton.y + 10, BLACK);
        DrawLine(plusButton.x + 10, plusButton.y + 5, plusButton.x + 10, plusButton.y + 15, BLACK);

        if (isMenuVisible) {
            for (int i = 0; i < 3; i++) {
                Color color = CheckCollisionPointRec(mouse, optionRects[i]) ? LIGHTGRAY : GRAY;
                DrawRectangleRec(optionRects[i], color);
                DrawText("Digital Clock", optionRects[i].x + 5, optionRects[i].y + 5, 20, BLACK);
            }
        }

        // debug draw physics bodies
        if (debugRender) {
            int bodiesCount = GetPhysicsBodiesCount();
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
        }



            // draw widgets
            for (int i = 0; i < myWidgets.count; i++) {
                if (myWidgets.array[i].dead) {continue;}
                Widget testWidget = myWidgets.array[i];
                PhysicsBody testBody = testWidget.body;

                // drawDigitalClock(&testWidget, &font);

                //drawGenericWidgetBG(&myWidgets.array[i]);
                testWidget.draw(&testWidget);
                if (debugRender) {
                    if (myWidgets.array[i].isGrabbed) {
                        DrawCircle(testBody->position.x, testBody->position.y, 10, RED);
                        DrawLine(testBody->position.x, testBody->position.y, testBody->position.x + testWidget.grabOffset.x, testBody->position.y + testWidget.grabOffset.y, ORANGE);
                    } else {
                        DrawCircle(testBody->position.x, testBody->position.y, 10, BLUE);

                    }
                }
            }

            
        DrawFPS(screenWidth - 90, screenHeight - 30);
        EndTextureMode();

        BeginDrawing(); // nobody touch this! Do your drawing in the BeginTextureMode above
        ClearBackground(BLACK);
        // DrawTexturePro(
        //     renderTexture.texture,
        //     (Rectangle){ 0, 0, (float)(renderTexture.texture.width), (float)(-renderTexture.texture.height) },
        //     (Rectangle){ 0, 0, (float)(GetScreenWidth()), (float)(GetScreenHeight()) },
        //     (Vector2){ 0, 0 },
        //     0,
        //     WHITE
        // );
        DrawTexturePro(renderTexture.texture, (Rectangle){ 0.0f, 0.0f, (float)renderTexture.texture.width, (float)-renderTexture.texture.height },
                       (Rectangle){ (GetScreenWidth() - ((float)screenWidth*scale))*0.5f, (GetScreenHeight() - ((float)screenHeight*scale))*0.5f,
                       (float)screenWidth*scale, (float)screenHeight*scale }, (Vector2){ 0, 0 }, 0.0f, WHITE);
        EndDrawing();
    }
    ClosePhysics();
    UnloadRenderTexture(renderTexture);
    CloseWindow();

    return 0;
}
