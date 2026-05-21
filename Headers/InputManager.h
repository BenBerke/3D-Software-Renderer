//
// Created by berke on 3/11/2026.
//

#ifndef INPUTMANAGER_H
#define INPUTMANAGER_H

#include <SDL3/SDL_scancode.h>

#include "Vector2.h"

typedef struct {
    bool prevKeyboardState[SDL_SCANCODE_COUNT];
    const bool* keyboardState;

    SDL_MouseButtonFlags mouseState;
    SDL_MouseButtonFlags prevMouseState;

    Vector2 mousePosition;
    Vector2 prevMousePosition;
} InputManager;

void input_manager_begin_frame(InputManager* inputManager);
bool input_manager_get_key(const InputManager* inputManager, SDL_Scancode key);
bool input_manager_get_key_down(const InputManager* inputManager, SDL_Scancode key);
bool input_manager_get_key_up(const InputManager* inputManager, SDL_Scancode key);

Vector2 input_manager_get_mouse_delta(const InputManager* inputManager);
Vector2 input_manager_get_mouse_position(const InputManager* inputManager);
bool input_manager_get_mouse(const InputManager* inputManager, Uint8 button);
bool input_manager_get_mouse_down(const InputManager* inputManager, Uint8 button);
bool input_manager_get_mouse_up(const InputManager* inputManager, Uint8 button);

#endif //INPUTMANAGER_H