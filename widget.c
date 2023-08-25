// #include "raylib.h"
// #include "stdio.h"

// #define PHYSAC_IMPLEMENTATION
// #include "physac.h"

struct simpleWidget {
    PhysicsBody body;
    int image;
};
typedef struct simpleWidget SimpleWidget;

void drawWidget(SimpleWidget *widget) {
    Vector2 pos = widget->body->position;
    DrawCircle(pos.x, pos.y, 10.0, BLUE);
}