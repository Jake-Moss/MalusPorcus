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

void drawDigitalClock(SimpleWidget *parent, Font font) {
    time_t rawtime;
    struct tm * timeinfo;

    time ( &rawtime );
    timeinfo = localtime ( &rawtime );

    Vector2 pos = parent->body->position;

    DrawTextEx(font, asctime (timeinfo), pos, font.baseSize*2.0f, 3, DARKPURPLE);
}
