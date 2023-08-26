#ifndef WIDGETS_H
#define WIDGETS_H

#include "raylib.h"
#include "physac.h"

#include "stddef.h"
#include "stdlib.h"

struct simpleWidget {
    PhysicsBody body;
    int image;
};
typedef struct simpleWidget SimpleWidget;

SimpleWidget newWidget(Vector2 location, Vector2 size, int image);

void drawWidget(SimpleWidget *widget);

struct WidgetArray {
    SimpleWidget *array;
    size_t used;
    size_t size;
};

typedef struct WidgetArray WidgetArray;

void initWidgetArray(WidgetArray *a, size_t initialSize);
void insertWidgetArray(WidgetArray *a, SimpleWidget element);
void freeWidgetArray(WidgetArray *a);

#endif
