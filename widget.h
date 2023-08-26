#ifndef WIDGETS_H
#define WIDGETS_H

#include "raylib.h"
#include "physac.h"

typedef struct textObj {
    Font textFont;
    char* textContent;
    Vector2 pos;
} TextObj;

typedef struct imageObj {
    Texture textureImage;
    Vector2 pos;
} ImageObj;

typedef struct renderPacket {
    ImageObj imageObj;
    TextObj textObj;

} RenderPacket;

typedef struct widget {
    PhysicsBody body;
    RenderPacket renderPacket;
    int image;
    // definitions of state (bools)
    bool isGrabbed;
} Widget;

Widget newWidget(Vector2 location, Vector2 size, int image);

void drawWidget(Widget *widget);

#endif
