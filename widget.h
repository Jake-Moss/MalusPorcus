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
    PhysicsBody body1;
    PhysicsBody body2;
    int image;
    Vector2 size; //width, height
    Vector2 grabOffset;
    bool isGrabbed;
    Font font; // hack job adding the font components into the structures
    void (*draw)(Widget*); // generic draw function pointer to be then defined for that pecific widget
    bool dead;
    void (*free)(Widget*);
    void *data;
};

void draw(Widget *widget);

Widget newWidget(Vector2 location, Vector2 size, int image);

void drawGenericWidgetBG(Widget *widget);

void drawWidget(Widget *widget);

void moveWhenGrabbed(Widget *widget, Vector2 mousePosition);

void widgetFree(Widget *widget);

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
