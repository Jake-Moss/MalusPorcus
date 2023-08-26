#include "widget.h"
#include "stdio.h"

#define MAX_SAMPLES               512
#define MAX_SAMPLES_PER_UPDATE   4096

#define MAX_TONE_GENS              1

void ToneGenAudioInputCallback(void *buffer, unsigned int frames);
Widget createToneGenerator(Vector2 initLoc, Vector2 size);
void drawToneGenerator(Widget *widget);
void enableTone(Widget *widget);
