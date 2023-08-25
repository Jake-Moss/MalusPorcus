#ifndef WIDGETS_H
#define WIDGETS_H

#include "raylib.h"
#include "physac.h"

struct simpleWidget {
    PhysicsBody body;
    int image;
};
typedef struct simpleWidget SimpleWidget;

SimpleWidget newWidget(Vector2 location, Vector2 size, int image);

void drawWidget(SimpleWidget *widget);

#endif
