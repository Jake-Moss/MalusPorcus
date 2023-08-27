#include "aquarium.h"
#include "widget.h"
#include "physac.h"
#include "helper.h"
#include <raylib.h>
#include <raymath.h>
#include <stdio.h>

// inputs: system time
//
// processing: - converting system time into a time string
//             - parsing into a RenderPacket object
//
// output:     - RenderPacket widget
//
Widget createAquarium(Vector2 initLoc, Vector2 size, Font font) {
    Widget aquarium = newWidget(initLoc, size, 5);
    DestroyPhysicsBody(aquarium.body);
    aquarium.body = CreatePhysicsBodyRectangle((Vector2){initLoc.x, initLoc.y + size.y/2.0}, size.x, 10, 1.0);
    aquarium.body1 = CreatePhysicsBodyRectangle((Vector2){initLoc.x - size.x/2.0, initLoc.y  - 10}, 10, size.y, 1.0);
    aquarium.body2 = CreatePhysicsBodyRectangle((Vector2){initLoc.x + size.x/2.0, initLoc.y  - 10}, 10, size.y, 1.0);
    aquarium.body->freezeOrient = true;
    aquarium.body1->freezeOrient = true;
    aquarium.body1->useGravity = false;
    aquarium.body2->freezeOrient = true;
    aquarium.body2->useGravity = false;
    aquarium.font = font;
    aquarium.draw = &drawAquarium;




    for (int i = 0; i < aquarium.body->shape.vertexData.vertexCount; i++) {
        printf("%.2f, %.2f\n", aquarium.body->shape.vertexData.positions[i].x, aquarium.body->shape.vertexData.positions[i].y);
        printf("%.2f, %.2f\n", aquarium.body->shape.vertexData.normals[i].x, aquarium.body->shape.vertexData.normals[i].y);
    }

    return aquarium;
}

void drawAquarium(Widget *widget) {

    Vector2 pos = widget->body->position;
    widget->body1->position = Vector2Add(pos, (Vector2){-widget->size.x/2.0, -widget->size.y/2.0 - 10});
    widget->body2->position = Vector2Add(pos, (Vector2){ widget->size.x/2.0, -widget->size.y/2.0 - 10});;

    //drawGenericWidgetBG(widget);
}

void freeAquarium(Widget *widget) {

    widgetFree(widget);
}
