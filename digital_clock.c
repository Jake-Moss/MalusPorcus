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

void drawDigitalClock(SimpleWidget *parent, Font *font) {
    time_t rawtime;
    struct tm * timeinfo;

    time ( &rawtime );
    timeinfo = localtime ( &rawtime );

    Vector2 pos = parent->body->position;
    Vector2 parentSize = parent->size;

    int fontSpacing = 3;
    float fontScale = 2.0f;
    // already function that does it for me!
    float fontCharSize = (font->baseSize)*fontScale;
    //int fontCharLength = fontCharSize * fontSpacing;
    //int strLength = 5;
    //pos.x = pos.x - (fontCharLength*strLength)/2;

    char buffer[6];
    strftime(buffer, 6, "%I:%M", timeinfo);

    Vector2 textSize = MeasureTextEx(*font, buffer, fontCharSize, fontSpacing);
    pos.x = pos.x - textSize.x/2;
    pos.y = pos.y - textSize.y/2;

    DrawTextEx(*font, buffer, pos, fontCharSize, fontSpacing, DARKPURPLE);
}
