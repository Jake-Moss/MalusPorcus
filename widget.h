#ifndef WIDGETS_H
#define WIDGETS_H

#include "raylib.h"
#include "physac.h"
#include "raymath.h"

#include "stddef.h"
#include "stdlib.h"

struct simpleWidget {
    PhysicsBody body;
    int image;
    Vector2 size; //width, height
    Vector2 grabOffset;
    bool isGrabbed;
};
typedef struct simpleWidget SimpleWidget;

SimpleWidget newWidget(Vector2 location, Vector2 size, int image);

void drawWidget(SimpleWidget *widget);

void moveWhenGrabbed(SimpleWidget *widget);

struct WidgetArray {
    SimpleWidget *array;
    size_t used;
    size_t size;
    int count;
};

typedef struct WidgetArray WidgetArray;

void initWidgetArray(WidgetArray *a, size_t initialSize);
void insertWidgetArray(WidgetArray *a, SimpleWidget element);
void freeWidgetArray(WidgetArray *a);

#endif
