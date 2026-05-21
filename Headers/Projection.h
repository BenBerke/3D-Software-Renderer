//
// Created by berke on 5/20/2026.
//

#ifndef SOFTWARERENDERER_PROJECTION_H
#define SOFTWARERENDERER_PROJECTION_H

#include <stdbool.h>
#include <SDL3/SDL_render.h>

#include "Vector3.h"
#include "Vector2.h"
#include "Texture.h"

typedef struct TexturedVertex {
    Vector2 position;
    Vector2 uv;

    float invZ;
} TexturedVertex;

typedef struct Triangle {
    TexturedVertex vertices[3];
    int minX, minY, maxX, maxY;
} Triangle;

typedef struct Rectangle {
    Vector2 vertices[4]; // top left, top right, bottom left, bottom right
} Rectangle;

typedef struct Quad {
    Vector3 vertices[4]; // top left, top right, bottom left, bottom right
    Texture texture;
} Quad;

float deg_to_rad(float deg);
float rad_to_deg(float rad);

Triangle create_triangle(Vector2 v1, Vector2 v2, Vector2 v3);

void render_triangle(uint32_t* framebuffer, const Triangle* tri, uint32_t color);

void render_rectangle(uint32_t* framebuffer, const Rectangle* square, uint32_t color);

void clear_depth_buffer(float* depthBuffer);

void render_textured_quad(
    float* depthBuffer,
    uint32_t *framebuffer,
    const Quad *quad,
    Vector3 cameraDir,
    Vector3 cameraPos,
    const Texture *texture);

#endif //SOFTWARERENDERER_PROJECTION_H