#include "texture.h"

#include "engine.h"

#include <stb_image/stb_image.h>


// Constructor & Destructor

EMBER_Texture* EMBER_TextureNew(const char* path, uint32_t filter) {

    // Get path
    String path_string = EMBER_GetRelativePath(path);

    // Allocate memory for texture
    EMBER_Texture* texture = (EMBER_Texture*) malloc(sizeof(EMBER_Texture));

    // Load the image
    stbi_set_flip_vertically_on_load(true);
	uint8_t* local_buffer = stbi_load(path_string.buffer, &texture->width, &texture->height, &texture->bpp, 4);

    if (!local_buffer) {
        printf("ERROR : Image file \"%s\" could not be read.\n", path_string.buffer);

        free(texture);
        return NULL;
    }

    glGenTextures(1, &texture->id);
    glBindTexture(GL_TEXTURE_2D, texture->id);

    uint32_t mipmap_filter = (filter == GL_LINEAR) ? GL_LINEAR_MIPMAP_LINEAR : GL_NEAREST_MIPMAP_NEAREST;
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, mipmap_filter);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, texture->width, texture->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, local_buffer);
    glGenerateMipmap(GL_TEXTURE_2D);

    glBindTexture(GL_TEXTURE_2D, 0);    
    stbi_image_free(local_buffer);

    return texture;
}

void EMBER_TextureFree(EMBER_Texture* texture) {
    if (!texture) {
        return;
    }

    glDeleteTextures(1, &texture->id);

    free(texture);
}


// Binding

void EMBER_TextureBind(const EMBER_Texture* texture, uint32_t slot) {

    glActiveTexture(GL_TEXTURE0 + slot);
    glBindTexture(GL_TEXTURE_2D, texture->id);
}

void EMBER_TextureUnbind() {

    glBindTexture(GL_TEXTURE_2D, 0);
}