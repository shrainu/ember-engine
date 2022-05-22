#include "input.h"

#include "window.h"


// Statics

static GLFWwindow* s_ember_window_ref_;
static vec2 s_ember_window_size_ref_;

static LIST_TYPE(EMBER_Event) s_ember_events_;


// Callback functions

void _EMBER_KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {

    EMBER_Event e = (EMBER_Event) {
        .type = EMBER_EVENT_KEY_PRESS,
        .key = key,
        .action = action,
    };

    LIST_PUSH(s_ember_events_, e);
}

void _EMBER_CharCallback(GLFWwindow* window, unsigned int codepoint) {

    EMBER_Event e = (EMBER_Event) {
        .type = EMBER_EVENT_CHAR_PRESS,
        .key = codepoint,
    };

    LIST_PUSH(s_ember_events_, e);
}

void _EMBER_MouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {

    EMBER_Event e = (EMBER_Event) {
        .type = EMBER_EVENT_MOUSE_CLICK,
        .button = button,
        .action = action,
    };

    LIST_PUSH(s_ember_events_, e);
}

void _EMBER_CursorPositionCallback(GLFWwindow* window, double xpos, double ypos) {

    EMBER_Event e = (EMBER_Event) {
        .type = EMBER_EVENT_CURSOR_MOVE,
        .x_change = xpos,
        .y_change = ypos,
    };

    LIST_PUSH(s_ember_events_, e);
}


// Gets

bool EMBER_GetKey(int32_t key) {
    return glfwGetKey(s_ember_window_ref_, key);
}

bool EMBER_GetMouseButton(int32_t button) {
    return glfwGetMouseButton(s_ember_window_ref_, button);
}

vec2s EMBER_GetCursorPos() {
    double x = 0;
    double y = 0;

    glfwGetCursorPos(s_ember_window_ref_, &x, &y);

    y = s_ember_window_size_ref_[1] - y;

    return (vec2s) {(float) x, (float) y};
}


// Events

void EMBER_PollEvents() {
    // Clear the events from last frame
    EMBER_ClearEvents();

    // Poll new events
    glfwPollEvents();
}

LIST_TYPE(EMBER_Event) EMBER_GetEvents() {
    return s_ember_events_;
}

void EMBER_ClearEvents() {
    LIST_CLEAR(s_ember_events_);
}


// Initialization & termination

void EMBER_InitInput() {

    // Set window reference
    s_ember_window_ref_ = EMBER_GLFWwindow();

    // Set window size reference
    vec2s temp = EMBER_GetWindowSize();

    s_ember_window_size_ref_[0] = temp.x;
    s_ember_window_size_ref_[1] = temp.y;

    // Create event list
    s_ember_events_ = LIST_NEW(s_ember_events_, EMBER_Event);

    // Set GLFW callbacks
    glfwSetKeyCallback(
        EMBER_GLFWwindow(), 
        _EMBER_KeyCallback
    );

    glfwSetCharCallback(
        EMBER_GLFWwindow(), 
        _EMBER_CharCallback
    );

    glfwSetMouseButtonCallback(
        EMBER_GLFWwindow(), 
        _EMBER_MouseButtonCallback
    );

    glfwSetCursorPosCallback(
        EMBER_GLFWwindow(), 
        _EMBER_CursorPositionCallback
    );
}

void EMBER_TerminateInput() {

    // Free event list
    LIST_FREE(s_ember_events_);
}