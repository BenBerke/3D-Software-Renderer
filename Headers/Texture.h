// Texture.h
#pragma once

#include <stdint.h>

typedef struct Texture {
    int width;
    int height;
    uint32_t* pixels;
} Texture;

Texture load_texture_from_file(const char* path);
void destroy_texture(Texture* texture);
uint32_t sample_texture(const Texture* texture, float u, float v);