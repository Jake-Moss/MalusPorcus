// #include "raylib.h"
// #include "stdio.h"

// #define PHYSAC_IMPLEMENTATION
// #include "physac.h"

struct simpleWidget {
    PhysicsBody body;
    int image;
};
typedef struct simpleWidget SimpleWidget;

SimpleWidget newWidget(Vector2 location, Vector2 size, int image) {
    PhysicsBody body = CreatePhysicsBodyRectangle(location, size.x, size.y, 1.0);
    
    SimpleWidget newWidget;
    newWidget.body = body;
    newWidget.image = image;

    return newWidget;
}

void drawWidget(SimpleWidget *widget) {
    Vector2 pos = widget->body->position;
    DrawCircle(pos.x, pos.y, 10.0, BLUE);
}