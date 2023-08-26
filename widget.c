#include "widget.h"
#include "stdio.h"

SimpleWidget newWidget(Vector2 location, Vector2 size, int image) {
    PhysicsBody body = CreatePhysicsBodyRectangle(location, size.x, size.y, 1.0);
    body->freezeOrient = true;
    SimpleWidget newWidget;
    newWidget.body = body;
    newWidget.image = image;

    return newWidget;
}

void drawWidget(SimpleWidget *widget) {
    Vector2 pos = widget->body->position;
    DrawCircle(pos.x, pos.y, 10.0, BLUE);
}






void initWidgetArray(WidgetArray *a, size_t initialSize) {
    a->array = malloc(initialSize * sizeof(SimpleWidget)); // should this be sizeof(SimpleWidget)?
    a->used = 0;
    a->size = initialSize;
}

void insertWidgetArray(WidgetArray *a, SimpleWidget element) {
    // a->used is the number of used entries, because a->array[a->used++] updates a->used only *after* the array has been accessed.
    // Therefore a->used can go up to a->size 
    if (a->used == a->size) {
        a->size *= 2;
        a->array = realloc(a->array, a->size * sizeof(SimpleWidget)); // as above?
    }
    a->array[a->used++] = element;
}

void freeWidgetArray(WidgetArray *a) {
    free(a->array);
    a->array = NULL;
    a->used = a->size = 0;
}