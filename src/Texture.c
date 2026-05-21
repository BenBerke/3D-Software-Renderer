//
// Created by berke on 5/21/2026.
//

#include "../Headers/Texture.h"

// Texture.c

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>

Texture load_texture_from_file(const char* path) {
    Texture texture = {0};

    SDL_Surface* loadedSurface = IMG_Load(path);

    if (loadedSurface == NULL) {
        fprintf(stderr, "IMG_Load failed: %s\n", SDL_GetError());
        return texture;
    }

    SDL_Surface* convertedSurface = SDL_ConvertSurface(
        loadedSurface,
        SDL_PIXELFORMAT_ARGB8888
    );

    SDL_DestroySurface(loadedSurface);

    if (convertedSurface == NULL) {
        fprintf(stderr, "SDL_ConvertSurface failed: %s\n", SDL_GetError());
        return texture;
    }

    texture.width = convertedSurface->w;
    texture.height = convertedSurface->h;
    texture.pixels = malloc(texture.width * texture.height * sizeof(uint32_t));

    if (texture.pixels == NULL) {
        fprintf(stderr, "Failed to allocate texture pixels\n");
        SDL_DestroySurface(convertedSurface);
        texture.width = 0;
        texture.height = 0;
        return texture;
    }

    uint8_t* sourcePixels = convertedSurface->pixels;
    const int sourcePitch = convertedSurface->pitch;

    for (int y = 0; y < texture.height; y++) {
        memcpy(
            &texture.pixels[y * texture.width],
            sourcePixels + y * sourcePitch,
            texture.width * sizeof(uint32_t)
        );
    }

    SDL_DestroySurface(convertedSurface);

    return texture;
}

void destroy_texture(Texture* texture) {
    if (texture == NULL) {
        return;
    }

    free(texture->pixels);

    texture->pixels = NULL;
    texture->width = 0;
    texture->height = 0;
}

uint32_t sample_texture(const Texture* texture, float u, float v) {
    if (texture == NULL || texture->pixels == NULL) {
        return 0xFFFF00FF; // bright magenta error colour
    }

    if (u < 0.0f) u = 0.0f;
    if (u > 1.0f) u = 1.0f;

    if (v < 0.0f) v = 0.0f;
    if (v > 1.0f) v = 1.0f;

    const int x = (int)(u * (float)(texture->width - 1));
    const int y = (int)(v * (float)(texture->height - 1));

    return texture->pixels[y * texture->width + x];
}
