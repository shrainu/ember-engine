#pragma once

#include "common.h"


// Character

typedef struct EMBER_Character {
    uint32_t id;
    vec2 size;
    vec2 bearing;
    uint32_t advance;
} EMBER_Character;


// Font

typedef struct EMBER_Font {
    String path;
    EMBER_Character characters[128];
} EMBER_Font;


// Short API typedef

#ifdef EMBER_SHORT_API

typedef EMBER_Font* Font;

#endif


// Constructor and destructor

EMBER_Font* EMBER_FontNew(const char* path, uint32_t size, uint32_t filter);

void EMBER_FontFree(EMBER_Font* font);


// Utility

vec2s EMBER_FontGetTextSize(EMBER_Font * font, const char* text, float scale);