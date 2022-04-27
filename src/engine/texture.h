#pragma once

#include "common.h"


// Texture 

typedef struct EMBER_Texture {
    uint32_t id;
    int32_t width;
    int32_t height;
    int32_t bpp;
} EMBER_Texture;


// Short API typedef

#ifdef EMBER_SHORT_API

typedef EMBER_Texture* Texture;

#endif


// Constructor & Destructor

/* Loads a texture from given path, user has to free the memory allocated for the texture. */
EMBER_Texture* EMBER_TextureNew(const char* path, uint32_t filter);

/* Frees the memory allocated for the texture. */
void EMBER_TextureFree(EMBER_Texture* texture);


// Binding

void EMBER_TextureBind(const EMBER_Texture* texture, uint32_t slot);

void EMBER_TextureUnbind();