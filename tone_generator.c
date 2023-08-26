#include "tone_generator.h"
#include "widget.h"

#define TONE_GENERATOR_BUTTON_SIZE (int)18
#define TONE_GENERATOR_BUTTON_MARGIN (int)2

// for audio stress
float toneGeneratorFrequencies[MAX_TONE_GENS] = {0.0};
float sinIdxs[MAX_TONE_GENS] = {0.0};

void ToneGenAudioInputCallback(void *buffer, unsigned int frames) {
    float audioFrequency[MAX_TONE_GENS];
    // make copy of tonGenFreqs
    for (int i = 0; i < MAX_TONE_GENS; i++) {
        audioFrequency[i] = toneGeneratorFrequencies[i];
    }
    // shift everything up and down 1Hz for *reasons*
    for (int i = 0; i < MAX_TONE_GENS; i++) {
        audioFrequency[i] = toneGeneratorFrequencies[i] + (audioFrequency[i] - toneGeneratorFrequencies[i]) * 0.95;
        audioFrequency[i] += 1.0;
        audioFrequency[i] -= 1.0;
    }

    // count non-zero frequencies
    int freqCount = 0;
    for (int i = 0; i < MAX_TONE_GENS; i++) {
        if (audioFrequency[i] != 0.0) freqCount++;
    }

    // calculate the increment for each generator
    float incr[MAX_TONE_GENS] = {0.0};
    for (int i = 0; i < MAX_TONE_GENS; i++) {
        incr[i] = audioFrequency[i] / 44100.0;
    }

    // buffer some audio so player doesn't cut in and out
    short *d = (short *)buffer;

    for (unsigned int i = 0; i < frames; i++)
    {
        float total = 0.0;
        for (int j = 0; j < MAX_TONE_GENS; j++) {
            total += sinf(2 * PI * sinIdxs[i]);
            sinIdxs[i] += incr[i];
            if (sinIdxs[i] > 1.0) sinIdxs[i] -= 1.0f;
        }
        float avg = 32000.0 * total / freqCount;
        
        d[i] = (short)(avg);
    }
}

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
        char *freqString = (char*)malloc(50 * sizeof(char));
        sprintf(freqString, "%f", toneGeneratorFrequencies[0]);
        DrawText(freqString, px, py - 50, 3, BLACK);
    } else {
        DrawCircle(px, py, TONE_GENERATOR_BUTTON_SIZE, LIGHTGRAY);
        char *freqString = (char*)malloc(50 * sizeof(char));
        sprintf(freqString, "%f", toneGeneratorFrequencies[0]);
        DrawText(freqString, px, py - 50, 3, BLACK);
    }
}

void enableTone(Widget *widget) {
    if (widget->turnedOn) {
        toneGeneratorFrequencies[0] = 440.0;
    }
}