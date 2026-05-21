//
// Created by berke on 3/11/2026.
//

#include <SDL3/SDL_events.h>
#include <SDL3/SDL_keyboard.h>
#include <SDL3/SDL_mouse.h>
#include <string.h> // Included for memcpy

#include "../Headers/InputManager.h"

void input_manager_begin_frame(InputManager* inputManager) {
    if (!inputManager->keyboardState) inputManager->keyboardState = SDL_GetKeyboardState(0);
    memcpy(inputManager->prevKeyboardState, inputManager->keyboardState, SDL_SCANCODE_COUNT * sizeof(bool));

    inputManager->prevMouseState = inputManager->mouseState;
    inputManager->prevMousePosition = inputManager->mousePosition;

    SDL_PumpEvents();

    inputManager->mouseState = SDL_GetMouseState(
        &inputManager->mousePosition.x,
        &inputManager->mousePosition.y
    );
}

bool input_manager_get_key(const InputManager* inputManager, SDL_Scancode key) {
    return inputManager->keyboardState[key];
}

bool input_manager_get_key_down(const InputManager* input_manager, SDL_Scancode key) {
    return input_manager->keyboardState[key] && !input_manager->prevKeyboardState[key];
}

bool input_manager_get_key_up(const InputManager* input_manager, SDL_Scancode key) {
    return !input_manager->keyboardState[key] && input_manager->prevKeyboardState[key];
}

Vector2 input_manager_get_mouse_position(const InputManager* inputManager) {
    return inputManager->mousePosition;
}

Vector2 input_manager_get_mouse_delta(const InputManager* inputManager) {
    return (Vector2){
        inputManager->mousePosition.x - inputManager->prevMousePosition.x,
        inputManager->mousePosition.y - inputManager->prevMousePosition.y
    };
}

bool input_manager_get_mouse(const InputManager* inputManager, Uint8 button) {
    return (inputManager->mouseState & SDL_BUTTON_MASK(button)) != 0;
}

bool input_manager_get_mouse_down(const InputManager* inputManager, Uint8 button) {
    return ((inputManager->mouseState & SDL_BUTTON_MASK(button)) != 0) &&
           ((inputManager->prevMouseState & SDL_BUTTON_MASK(button)) == 0);
}

bool input_manager_get_mouse_up(const InputManager* inputManager, Uint8 button) {
    return ((inputManager->mouseState & SDL_BUTTON_MASK(button)) == 0) &&
           ((inputManager->prevMouseState & SDL_BUTTON_MASK(button)) != 0);
}