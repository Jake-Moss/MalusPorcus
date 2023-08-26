#include "digital_clock.h"
#include "widget.h"
#include <time.h>


// inputs: system time
//
// processing: - converting system time into a time string
//             - parsing into a RenderPacket object
//
// output:     - RenderPacket widget
//
Widget createDigitalClock(Vector2 initLoc, Vector2 size, Font font) {
    // makes a widget,
    Widget digitalClockWidget = newWidget(initLoc, size, 1);
    digitalClockWidget.font = font;
    digitalClockWidget.draw = &drawDigitalClock; // reassign draw function over here
    return digitalClockWidget;
}

void drawDigitalClock(Widget *widget) {
    time_t rawtime;
    struct tm * timeinfo;

    time ( &rawtime );
    timeinfo = localtime ( &rawtime );


    drawGenericWidgetBG(widget);

    Vector2 pos = widget->body->position;
    Vector2 parentSize = widget->size;

    int yPadding = 30;
    int xPadding = 30;

    int innerRectWidth = parentSize.x - xPadding;
    int innerRectHeight = parentSize.y - yPadding;
    Rectangle innerRect = {pos.x - innerRectWidth/2, pos.y - innerRectHeight/2, innerRectWidth, innerRectHeight};
    DrawRectangleRounded(innerRect, 0.15, 100, WHITE);

    int fontSpacing = 2;
    float fontScale = 2.7f;
    // already function that does it for me!
    float fontCharSize = (widget->font.baseSize)*fontScale;
    //int fontCharLength = fontCharSize * fontSpacing;
    //int strLength = 5;
    //pos.x = pos.x - (fontCharLength*strLength)/2;

    char buffer[6];
    strftime(buffer, 6, "%I:%M", timeinfo);

    Vector2 textSize = MeasureTextEx(widget->font, buffer, fontCharSize, fontSpacing);
    pos.x = pos.x - textSize.x/2;
    pos.y = pos.y - textSize.y/2;

    DrawTextEx(widget->font, buffer, pos, fontCharSize, fontSpacing, BLACK);
}
