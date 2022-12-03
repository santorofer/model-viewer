#include "texture.h"

#include <assert.h>
#include <stdint.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

GLuint load_texture(const char * filename)
{
    assert(filename);

    int width;
    int height;
    int channels;

    // Load raw RGBA pixel data from the image file
    uint8_t * data = stbi_load(filename, &width, &height, &channels, STBI_rgb_alpha);
    if (!data) {
        return 0;
    }

    GLuint id;

    // Generate a blank texture
    glGenTextures(1, &id);

    // Bind it to GL_TEXTURE_2D so all following commands affect this texture
    glBindTexture(GL_TEXTURE_2D, id);

    // Configure wrapping settings, pixels outside of ([0,1], [0,1]) will return the pixel value at the edge
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    // Configure min/mag filters to use linear sampling
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Upload the pixel data to the driver
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

    return id;
}