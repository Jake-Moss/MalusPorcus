#ifndef WIDGETS_H
#define WIDGETS_H

#include "raylib.h"
#include "physac.h"
#include "raymath.h"

#include "stddef.h"
#include "stdlib.h"
//


typedef struct widget Widget;
struct widget {
    PhysicsBody body;
    int image;
    Vector2 size; //width, height
    Vector2 grabOffset;
    bool isGrabbed;
    void (*draw)(Widget*); // generic draw function pointer
                                             // to be then defined for that
                                             // specific widget
};

void draw(Widget *widget);

Widget newWidget(Vector2 location, Vector2 size, int image);

void drawWidget(Widget *widget);

void moveWhenGrabbed(Widget *widget);

struct WidgetArray {
    Widget *array;
    size_t size;
    size_t count;
};

typedef struct WidgetArray WidgetArray;

void initWidgetArray(WidgetArray *a, size_t initialSize);
void insertWidgetArray(WidgetArray *a, Widget element);
void freeWidgetArray(WidgetArray *a);

#endif
