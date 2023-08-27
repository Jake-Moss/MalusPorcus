#include "widget.h"
#include "stdio.h"

Widget newWidget(Vector2 location, Vector2 size, int image) {
    PhysicsBody body = CreatePhysicsBodyRectangle(location, size.x, size.y, 1.0);
    body->freezeOrient = true;
    Widget newWidget;
    newWidget.size = size;
    newWidget.body = body;
    newWidget.body1 = NULL;
    newWidget.body2 = NULL;
    newWidget.image = image;
    newWidget.isGrabbed = false;
    newWidget.grabOffset = (Vector2) {0.0, 0.0};
    newWidget.draw = &draw;
    newWidget.dead = false;
    newWidget.free = &widgetFree;
    newWidget.data = NULL;
    return newWidget;
}

void draw(Widget *widget) {
    Vector2 pos = widget->body->position;
    DrawCircle(pos.x, pos.y, 5.0, GREEN);
}

void drawWidget(Widget *widget) {
    Vector2 pos = widget->body->position;
    DrawCircle(pos.x, pos.y, 10.0, BLUE);
}

void moveWhenGrabbed(Widget *widget, Vector2 mousePosition) {
    // check if the widget is currently grabbed - if so, move towards the mouse + graboffset (using a lot of impulse)

    if (widget->isGrabbed) {
        Vector2 grabOffsetMouseDifference = Vector2Subtract(Vector2Subtract(mousePosition, widget->grabOffset), widget->body->position);
        PhysicsAddForce(widget->body, Vector2Scale(grabOffsetMouseDifference, Vector2LengthSqr(grabOffsetMouseDifference)));
        widget->body->velocity = Vector2Scale(widget->body->velocity, 0.9);
    }
}

void drawGenericWidgetBG(Widget *widget) {
    Vector2 centerPos = widget->body->position;
    Vector2 size = widget->size;

    float width = size.x;
    float height = size.y;

    float px = centerPos.x - width / 2;
    float py = centerPos.y - height / 2;

    int footHeight = 10;
    int footWidth = 35;

    int borderThickness = 5;

    Rectangle rect = {px + borderThickness, py + borderThickness, width - 2 * borderThickness, height - footHeight - 2 * borderThickness};


    Color keyColour = DARKPURPLE;
    Color highColour = ColorBrightness(keyColour, 0.5);
    Color lowColour = ColorBrightness(keyColour, -0.1);

    // main body
    DrawRectangleRounded(rect, 0.15, 100, keyColour);
    DrawRectangleRoundedLines(rect, 0.15, 100, 5, highColour);
    DrawRectangle(rect.x + 2 * borderThickness, rect.y + rect.height, rect.width - 4 * borderThickness, borderThickness, lowColour);
    // DrawRectangle(px, py, width, height - footHeight, PINK);
    // feet
    // DrawRectangle(px, py + height - footHeight, width, footHeight, BLUE);
    // left foot
    DrawRectangle(px + footWidth / 2, py + height - footHeight, footWidth, footHeight / 2, BLACK);
    DrawRectangle(px + (footWidth + footHeight) / 2, py + height - footHeight / 2, footWidth - footHeight, footHeight / 2, BLACK);
    // right foot
    DrawRectangle(px + width - footWidth / 2 - footWidth, py + height - footHeight, footWidth, footHeight / 2, BLACK);
    DrawRectangle(px + width - (footWidth + footHeight) / 2 - (footWidth - footHeight), py + height - footHeight / 2, footWidth - footHeight, footHeight / 2, BLACK);
}

void widgetFree(Widget *widget) {
    widget->dead = true;
    DestroyPhysicsBody(widget->body);
}

void initWidgetArray(WidgetArray *a, size_t initialSize) {
    a->array = malloc(initialSize * sizeof(Widget));
    a->size = initialSize;
    a->count = 0;
}

void insertWidgetArray(WidgetArray *a, Widget element) {
    // a->used is the number of used entries, because a->array[a->used++] updates a->used only *after* the array has been accessed.
    // Therefore a->used can go up to a->size 
    if (a->count == a->size) {
        a->size *= 2;
        a->array = realloc(a->array, a->size * sizeof(Widget)); // as above?
    }
    a->array[a->count++] = element;
}


void freeWidgetArray(WidgetArray *a) {

    free(a->array);
    a->array = NULL;
    a->count = a->size = 0;
}
