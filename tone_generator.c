#include "tone_generator.h"
#include "widget.h"

#define TONE_GENERATOR_BUTTON_SIZE (int)18
#define TONE_GENERATOR_BUTTON_MARGIN (int)2

Widget createToneGenerator(Vector2 initLoc, Vector2 size) {
    // makes a widget,
    Widget toneGeneratorWidget = newWidget(initLoc, size, 1);
    toneGeneratorWidget.draw = &drawToneGenerator; // reassign draw function over here
    toneGeneratorWidget.turnedOn = false;
    toneGeneratorWidget.hasButton = TONE_GENERATOR_BUTTON_SIZE;
    return toneGeneratorWidget;
}

void drawToneGenerator(Widget *widget) {
    drawGenericWidgetBG(widget);

    Vector2 pos = widget->body->position;
    float px = pos.x;
    float py = pos.y;

    // draw button
    DrawCircle(px, py, TONE_GENERATOR_BUTTON_SIZE + TONE_GENERATOR_BUTTON_MARGIN, BLACK);
    if (widget->turnedOn) {
        DrawCircle(px, py, TONE_GENERATOR_BUTTON_SIZE, GREEN);
    } else {
        DrawCircle(px, py, TONE_GENERATOR_BUTTON_SIZE, LIGHTGRAY);
    }
}
