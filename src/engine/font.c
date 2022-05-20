#include "font.h"

#include <freetype/freetype.h>


// Constructor and Destructor

EMBER_Font* EMBER_FontNew(const char* path, uint32_t size, uint32_t filter) {

    FILE* f = NULL;
    if (!(f = fopen(path, "r"))) {
        printf("ERROR: File could not be opened to load font '%s'.\n", path);
        return NULL;
    }
    fclose(f);

    FT_Library ft;
    if (FT_Init_FreeType(&ft)) {
        printf("ERROR: FreeType could not be initialized.\n");
        return NULL;
    }

    FT_Face face;
    if (FT_New_Face(ft, path, 0, &face)) {
        printf("ERROR: Failed to load font '%s'.\n", path);
        return NULL;
    }
    FT_Set_Pixel_Sizes(face, 0, size); // Width calculated automatically

    // Allocate memory for the font
    EMBER_Font* font = (EMBER_Font*) malloc(sizeof(EMBER_Font));

    // Disable byte alignment restrictions
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    for (unsigned char c = 0; c < 128; ++c) {

        // Load the character
        if (FT_Load_Char(face, c, FT_LOAD_RENDER)) {
            printf("WARNING: Failed to load glyph '%c'.\n", c);
            continue;
        }

        // Generate the texture
        uint32_t id;

        glGenTextures(1, &id);
        glBindTexture(GL_TEXTURE_2D, id);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, (int32_t) filter);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, (int32_t) filter);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        glTexImage2D(
            GL_TEXTURE_2D,
            0,
            GL_RED,
            (int32_t) face->glyph->bitmap.width,
            (int32_t) face->glyph->bitmap.rows,
            0,
            GL_RED,
            GL_UNSIGNED_BYTE,
            face->glyph->bitmap.buffer
        );

        // Unbind the texture
        glBindTexture(GL_TEXTURE_2D, 0);

        // Create character
        EMBER_Character chr = (EMBER_Character) {
            .id = id,
            .size = {
                (float) face->glyph->bitmap.width,
                (float) face->glyph->bitmap.rows
            },
            .bearing = {
                (float) face->glyph->bitmap_left,
                (float) face->glyph->bitmap_top
            },
            .advance = face->glyph->advance.x >> 6,
        };

        // Add character to font array
        font->characters[c] = chr;
    }

    // Add font path to font
    font->path = STRING_NEW(path);

    // Cleanup
    FT_Done_Face(face);
    FT_Done_FreeType(ft);

    return font;
}

void EMBER_FontFree(EMBER_Font* font) {
    if (!font) {
        return;
    }

    // Free the font path
    STRING_FREE(&font->path);

    // Free the font
    free(font);
}


// Utilities

vec2s EMBER_FontGetTextSize(EMBER_Font * font, const char* text, float scale) {

    float size_x = 0.0f;
    float size_y = 0.0f;

    uint32_t len = strlen(text);

    for (const char* c = text; c < text + len; ++c) {

        // Get the character from font
        const EMBER_Character* chr = &font->characters[*c];

        // float width  = chr->size[0] * scale; TODO : Remove this line.
        float height = chr->bearing[1] * scale;

        float advance = (float)chr->advance * scale;

        if (height >= size_y) {
            size_y = height;
        }

        if (c == text + len - 1) {
            size_x += (chr->size[0] + chr->bearing[0]) * scale;
        } else {
            size_x += advance;
        }
    }

    return (vec2s) { size_x, size_y };
}