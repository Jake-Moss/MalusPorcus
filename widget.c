#include "widget.h"
#include "stdio.h"

Widget newWidget(Vector2 location, Vector2 size, int image) {
    PhysicsBody body = CreatePhysicsBodyRectangle(location, size.x, size.y, 1.0);
    
    Widget newWidget;
    newWidget.body = body;
    newWidget.image = image;

    return newWidget;
}

void drawWidget(Widget *widget) {
    Vector2 pos = widget->body->position;
    DrawCircle(pos.x, pos.y, 10.0, BLUE);
}
