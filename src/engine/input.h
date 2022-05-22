#pragma once

#include "common.h"


// Event types

enum EMBER_EventType {
    EMBER_EVENT_KEY_PRESS,
    EMBER_EVENT_CHAR_PRESS,
    EMBER_EVENT_MOUSE_CLICK,
    EMBER_EVENT_CURSOR_MOVE,
};


// Event actions

enum EMBER_EventAction {
    EMBER_RELEASE,
    EMBER_PRESS,
    EMBER_REPEAT
};


// Type

typedef struct EMBER_Event {
    // Type
    int32_t type;

    // Cursor movement
    float x_change;
    float y_change;

    // Key
    uint32_t key;

    // Button
    uint32_t button;

    // Action
    uint32_t action;
} EMBER_Event;


// Declare List

LIST_DEFINE(EMBER_Event);


// Callback functions

void _EMBER_KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

void _EMBER_CharCallback(GLFWwindow* window, unsigned int codepoint);

void _EMBER_MouseButtonCallback(GLFWwindow* window, int button, int action, int mods);

void _EMBER_CursorPositionCallback(GLFWwindow* window, double xpos, double ypos);


// Gets

bool EMBER_GetKey(int32_t key);

bool EMBER_GetMouseButton(int32_t button);

vec2s EMBER_GetCursorPos();


// Events

void EMBER_PollEvents();

LIST_TYPE(EMBER_Event) EMBER_GetEvents();

void EMBER_ClearEvents();


// Initialization & termination

void EMBER_InitInput();

void EMBER_TerminateInput();