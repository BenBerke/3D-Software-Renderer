#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>

#include "Headers/config.h"

#include "Headers/InputManager.h"
#include "Headers/Projection.h"
#include "Headers/Vector3.h"
#include "Headers/Texture.h"

static uint32_t make_colour(uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
    return ((uint32_t)a << 24) |
           ((uint32_t)r << 16) |
           ((uint32_t)g << 8)  |
           ((uint32_t)b);
}

static void clear_framebuffer(uint32_t* framebuffer, uint32_t colour) {
    for (int i = 0; i < SCREEN_WIDTH * SCREEN_HEIGHT; i++) {
        framebuffer[i] = colour;
    }
}

static void generate_cube(Quad* outQuads, int startIndex, Vector3 offset, float size, Texture topTexture, Texture bottomTexture, Texture sideTexture) {
    float half = size / 2.0f;

    // --- FACE 1: FRONT ---
    outQuads[startIndex + 0].vertices[0] = (Vector3){-half + offset.x,  half + offset.y, -half + offset.z};
    outQuads[startIndex + 0].vertices[1] = (Vector3){ half + offset.x,  half + offset.y, -half + offset.z};
    outQuads[startIndex + 0].vertices[2] = (Vector3){-half + offset.x, -half + offset.y, -half + offset.z};
    outQuads[startIndex + 0].vertices[3] = (Vector3){ half + offset.x, -half + offset.y, -half + offset.z};

    outQuads[startIndex + 0].texture = sideTexture;

    // --- FACE 2: BACK ---
    outQuads[startIndex + 1].vertices[0] = (Vector3){ half + offset.x,  half + offset.y,  half + offset.z};
    outQuads[startIndex + 1].vertices[1] = (Vector3){-half + offset.x,  half + offset.y,  half + offset.z};
    outQuads[startIndex + 1].vertices[2] = (Vector3){ half + offset.x, -half + offset.y,  half + offset.z};
    outQuads[startIndex + 1].vertices[3] = (Vector3){-half + offset.x, -half + offset.y,  half + offset.z};

    outQuads[startIndex + 1].texture = sideTexture;

    // --- FACE 3: LEFT ---
    outQuads[startIndex + 2].vertices[0] = (Vector3){-half + offset.x,  half + offset.y,  half + offset.z};
    outQuads[startIndex + 2].vertices[1] = (Vector3){-half + offset.x,  half + offset.y, -half + offset.z};
    outQuads[startIndex + 2].vertices[2] = (Vector3){-half + offset.x, -half + offset.y,  half + offset.z};
    outQuads[startIndex + 2].vertices[3] = (Vector3){-half + offset.x, -half + offset.y, -half + offset.z};

    outQuads[startIndex + 2].texture = sideTexture;

    // --- FACE 4: RIGHT ---
    outQuads[startIndex + 3].vertices[0] = (Vector3){ half + offset.x,  half + offset.y, -half + offset.z};
    outQuads[startIndex + 3].vertices[1] = (Vector3){ half + offset.x,  half + offset.y,  half + offset.z};
    outQuads[startIndex + 3].vertices[2] = (Vector3){ half + offset.x, -half + offset.y, -half + offset.z};
    outQuads[startIndex + 3].vertices[3] = (Vector3){ half + offset.x, -half + offset.y,  half + offset.z};

    outQuads[startIndex + 3].texture = sideTexture;

    // --- FACE 5: TOP ---
    outQuads[startIndex + 4].vertices[0] = (Vector3){-half + offset.x,  half + offset.y,  half + offset.z};
    outQuads[startIndex + 4].vertices[1] = (Vector3){ half + offset.x,  half + offset.y,  half + offset.z};
    outQuads[startIndex + 4].vertices[2] = (Vector3){-half + offset.x,  half + offset.y, -half + offset.z};
    outQuads[startIndex + 4].vertices[3] = (Vector3){ half + offset.x,  half + offset.y, -half + offset.z};

    outQuads[startIndex + 4].texture = topTexture;

    // --- FACE 6: BOTTOM ---
    outQuads[startIndex + 5].vertices[0] = (Vector3){-half + offset.x, -half + offset.y, -half + offset.z};
    outQuads[startIndex + 5].vertices[1] = (Vector3){ half + offset.x, -half + offset.y, -half + offset.z};
    outQuads[startIndex + 5].vertices[2] = (Vector3){-half + offset.x, -half + offset.y,  half + offset.z};
    outQuads[startIndex + 5].vertices[3] = (Vector3){ half + offset.x, -half + offset.y,  half + offset.z};

    outQuads[startIndex + 5].texture = bottomTexture;
}

int main(void) {
    if (SDL_Init(SDL_INIT_VIDEO) == false) {
        fprintf(stderr, "SDL_Init Error: %s\n", SDL_GetError());
        return 1;
    }
    SDL_Window* window = NULL;
    SDL_Renderer* renderer = NULL;
    if (SDL_CreateWindowAndRenderer("Software Renderer", SCREEN_WIDTH, SCREEN_HEIGHT, 0, &window, &renderer) == false) {
        fprintf(stderr, "Window/Renderer Error: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }
    uint32_t* framebuffer = malloc(SCREEN_WIDTH * SCREEN_HEIGHT * sizeof(uint32_t));

    if (framebuffer == NULL) {
        fprintf(stderr, "Failed to allocate framebuffer\n");
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }
    SDL_Texture* framebufferTexture = SDL_CreateTexture(
              renderer,
        SDL_PIXELFORMAT_ARGB8888,
        SDL_TEXTUREACCESS_STREAMING,
            SCREEN_WIDTH,
            SCREEN_HEIGHT
    );
    if (framebufferTexture == NULL) {
        fprintf(stderr, "Failed to create texture: %s\n", SDL_GetError());
        free(framebuffer);
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }
    float* depthBuffer = malloc(SCREEN_WIDTH * SCREEN_HEIGHT * sizeof(float));

    if (depthBuffer == NULL) {
        fprintf(stderr, "Failed to allocate depth buffer\n");
        return 1;
    }

    SDL_SetWindowRelativeMouseMode(window, true);

    Vector3 cameraPos = {0.0f, 0.0f, 0.0f};
    Vector3 cameraDirDeg = {0.0f, 0.0f, .0f};

    Texture testTexture = load_texture_from_file("C:/Users/berke/Documents/Tilky Engine/Projects/luatest/Assets/Textures/wall.png");
    Texture grass = load_texture_from_file("C:/Users/berke/Desktop/grass.png");
    Texture dirt = load_texture_from_file("C:/Users/berke/Desktop/dirt.png");
    // 3 cubes * 6 faces per cube = 18 quads total
    const int totalQuads = 18;
    Quad sceneQuads[totalQuads];

    // Cube 1: Left side
    generate_cube(sceneQuads, 0, (Vector3){-150.0f, 0.0f, 350.0f}, 80.0f, grass, dirt, dirt);

    // Cube 2: Center
    generate_cube(sceneQuads, 6, (Vector3){0.0f, 0.0f, 350.0f}, 80.0f, grass, dirt, dirt);

    // Cube 3: Right side
    generate_cube(sceneQuads, 12, (Vector3){150.0f, 0.0f, 350.0f}, 80.0f, grass, dirt, dirt);

    const uint32_t white = make_colour(255, 255, 255, 255);
    const uint32_t black = make_colour(0, 0, 0, 255);

    InputManager inputManager = {0};
    bool running = true;
    while (running) {
        input_manager_begin_frame(&inputManager);
        float dx = 0.0f;
        float dy = 0.0f;
        SDL_GetRelativeMouseState(&dx, &dy);

        if (input_manager_get_key_down(&inputManager, SDL_SCANCODE_ESCAPE)) running = false;

        const float moveSpeed = 3.0f;

        // cameraDirDeg.y = yaw
        const float yawRad = deg_to_rad(cameraDirDeg.y);

        // Forward direction: when yaw = 0, forward is +Z
        const Vector3 forward = {
            sinf(yawRad),
            0.0f,
            cosf(yawRad)
        };

        // Right direction
        const Vector3 right = {
            cosf(yawRad),
            0.0f,
            -sinf(yawRad)
        };

        float moveForward = 0.0f;
        float moveRight = 0.0f;

        if (input_manager_get_key(&inputManager, SDL_SCANCODE_W)) moveForward += 1.0f;
        if (input_manager_get_key(&inputManager, SDL_SCANCODE_S)) moveForward -= 1.0f;
        if (input_manager_get_key(&inputManager, SDL_SCANCODE_D)) moveRight += 1.0f;
        if (input_manager_get_key(&inputManager, SDL_SCANCODE_A)) moveRight -= 1.0f;

        // Prevent diagonal movement being faster
        float length = sqrtf(moveForward * moveForward + moveRight * moveRight);

        if (length > 0.0f) {
            moveForward /= length;
            moveRight /= length;
        }

        cameraPos.x += (forward.x * moveForward + right.x * moveRight) * moveSpeed;
        cameraPos.z += (forward.z * moveForward + right.z * moveRight) * moveSpeed;

        // Vertical movement
        if (input_manager_get_key(&inputManager, SDL_SCANCODE_SPACE)) {
            cameraPos.y += moveSpeed;
        }

        if (input_manager_get_key(&inputManager, SDL_SCANCODE_LCTRL)) {
            cameraPos.y -= moveSpeed;
        }

        const float mouseSensitivity = 0.1f;

        cameraDirDeg.y += dx * mouseSensitivity;
        cameraDirDeg.x -= -dy * mouseSensitivity;

        if (cameraDirDeg.x > 89.0f) cameraDirDeg.x = 89.0f;
        if (cameraDirDeg.x < -89.0f) cameraDirDeg.x = -89.0f;

        Vector3 cameraDirRad =
        {deg_to_rad(cameraDirDeg.x), deg_to_rad(cameraDirDeg.y), deg_to_rad(cameraDirDeg.z)};


        clear_framebuffer(framebuffer, white);
        clear_depth_buffer(depthBuffer);

        for (int i = 0; i < sizeof(sceneQuads) / sizeof(sceneQuads[0]); i++) {
            render_textured_quad(depthBuffer, framebuffer, &sceneQuads[i], cameraDirRad,cameraPos, &sceneQuads[i].texture);
        }

        SDL_UpdateTexture(
            framebufferTexture,
            NULL,
            framebuffer,
        SCREEN_WIDTH * sizeof(uint32_t)
        );

        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderClear(renderer);

        SDL_RenderTexture(renderer, framebufferTexture, NULL, NULL);
        SDL_RenderPresent(renderer);
    }

    destroy_texture(&testTexture);
    free(depthBuffer);
    SDL_DestroyTexture(framebufferTexture);
    free(framebuffer);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}