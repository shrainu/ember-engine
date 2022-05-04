#include "window.h"

#include "engine.h"

#include "ember-std/parser/parser.h"


// Properties

static String s_ember_app_name_;
static int32_t s_ember_window_width_;
static int32_t s_ember_window_height_;
static bool s_ember_vsync_;
static bool s_ember_retina_;
static int32_t s_ember_window_mode_;
static bool s_ember_maximize_;
static bool s_ember_resizable_;
static int32_t s_ember_monitor_index_;


// Window

static GLFWwindow* s_ember_glfw_window_;
static int32_t s_ember_refresh_rate_;


// Monitor

static GLFWmonitor* s_ember_monitor_;
static int32_t s_ember_monitor_count_;


// Initialization

void EMBER_InitWindowProperties() {

    String config_path = EMBER_GetConfigPath();
    
    // Read the config file
    Map* map = EMBER_ParseYaml(config_path.buffer);
    
    if (!map) {
        printf("ERROR: Could not parse the config file of the app.\n");
        abort();
    } else {
        STRING_FREE(&config_path);
    }

    // Set window properties
    s_ember_app_name_ = MAP_PARSE_STRING(map, "name");

    s_ember_window_width_ = MAP_PARSE_INT(map, "width");

    s_ember_window_height_ = MAP_PARSE_INT(map, "height");

    s_ember_vsync_ = MAP_PARSE_BOOL(map, "vsync");

    s_ember_refresh_rate_ = MAP_PARSE_INT(map, "refresh-rate");

    s_ember_retina_ = MAP_PARSE_BOOL(map, "retina");

    s_ember_window_mode_ = MAP_PARSE_INT(map, "window-mode");

    s_ember_maximize_ = MAP_PARSE_BOOL(map, "maximize");

    s_ember_resizable_ = MAP_PARSE_BOOL(map, "resizable");

    s_ember_monitor_index_ = MAP_PARSE_INT(map, "monitor");

    // Free the map
    MAP_FREE(map);
}

bool EMBER_InitWindow() {

    // Init properties
    EMBER_InitWindowProperties();

    // Init GLFW
    if (!glfwInit()) {
        printf("ERROR: GLFW could not be initialized!\n");
        return false;
    }

    // GL window hints
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // Window hints
    if (s_ember_resizable_) {
        glfwWindowHint(GLFW_RESIZABLE, (int) s_ember_resizable_);
    }
    if (s_ember_maximize_) {
        glfwWindowHint(GLFW_MAXIMIZED, (int) s_ember_maximize_);
    }

    // Create window
    s_ember_glfw_window_ = glfwCreateWindow(
        s_ember_window_width_, 
        s_ember_window_height_, 
        s_ember_app_name_.buffer, 
        NULL, NULL
    );
    
    if (!s_ember_glfw_window_) {
        printf("ERROR: GLFW window could not be created!\n");
        
        glfwTerminate();
        return false;
    }
    
    // Set monitor
    EMBER_SetMonitor(s_ember_monitor_index_);

    // Set Retina
    EMBER_SetRetinaMode(s_ember_retina_);

    // Set refresh rate
    EMBER_SetRefreshRate(s_ember_refresh_rate_);

    // Set window mode
    EMBER_SetWindowMode(s_ember_window_mode_);

    // Make context current
    glfwMakeContextCurrent(s_ember_glfw_window_);

    // Initialize GLEW
    if(glewInit()) {
        printf("ERROR: GLEW could not be initialized!\n");

        glfwTerminate();
        return false;
    }

    // Set VSync
    EMBER_SetVSync(s_ember_vsync_);

    // Set GL Viewport
    EMBER_SetGLViewport(0, 0, s_ember_window_width_, s_ember_window_height_);

    return true;
}

void EMBER_TerminateWindow() {

    // Terminate GLFW
    glfwTerminate();

    // Free the app name
    STRING_FREE(&s_ember_app_name_);
}


// Window

GLFWwindow* EMBER_GLFWwindow() {
    return s_ember_glfw_window_;
}

bool EMBER_WindowShouldClose() {
    return glfwWindowShouldClose(s_ember_glfw_window_);
}

void EMBER_SetWindowShouldClose(bool value) {
    return glfwSetWindowShouldClose(s_ember_glfw_window_, (int) value);
}

void EMBER_SwapBuffers() {
    glfwSwapBuffers(s_ember_glfw_window_);   
}

void EMBER_ClearColor(float r, float g, float b) {
    glClearColor(r, g, b, 1.0f);
}

void EMBER_Clear(uint32_t flags) {
    glClear(flags);
}


// Apply

void EMBER_ApplyWindowSettings() {

    EMBER_SetWindowMode(s_ember_window_mode_);
}


// Window Size

vec2s EMBER_GetWindowSize() {
    return (vec2s) {(float)s_ember_window_width_, (float)s_ember_window_height_};
}


// Window Mode

void EMBER_SetWindowMode(int32_t mode) {

    s_ember_window_mode_ = mode;

    const GLFWvidmode* video_mode = glfwGetVideoMode(s_ember_monitor_);

    GLFWmonitor* monitor = NULL;
    int32_t posx = 0;
    int32_t posy = 0;

    if (s_ember_window_mode_ == EMBER_WINDOW_MODE_WINDOWED | 
        s_ember_window_mode_ == EMBER_WINDOW_MODE_WINDOWED_FULLSCREEN) {
        posx = (video_mode->width  - (int32_t) s_ember_window_width_)  / 2;
        posy = (video_mode->height - (int32_t) s_ember_window_height_) / 2;
    }
    
    if (s_ember_window_mode_ == EMBER_WINDOW_MODE_WINDOWED_FULLSCREEN) {
        s_ember_window_width_  = video_mode->width;
        s_ember_window_height_ = video_mode->height;
    }
    
    if (s_ember_window_mode_ == EMBER_WINDOW_MODE_FULLSCREEN) {
        monitor = s_ember_monitor_;

        if (s_ember_maximize_) {
            s_ember_window_width_  = video_mode->width;
            s_ember_window_height_ = video_mode->height;   
        }
    }

    glfwSetWindowMonitor(
        s_ember_glfw_window_,
        monitor,
        posx, 
        posy,
        s_ember_window_width_,
        s_ember_window_height_,
        s_ember_refresh_rate_
    );
}

int32_t EMBER_GetWindowMode() {
    return s_ember_window_mode_;
}


// Monitor

void EMBER_SetMonitor(uint32_t monitor) {

    GLFWmonitor** monitors = EMBER_GetAvailableMonitors();

    s_ember_monitor_index_ = (int32_t) monitor;
    s_ember_monitor_ = monitors[s_ember_monitor_index_];
}

GLFWmonitor* EMBER_GetMonitor() {
    return s_ember_monitor_;
}

int32_t EMBER_GetMonitorIndex() {
    return s_ember_monitor_index_;
}

int32_t EMBER_GetMonitorCount() {

    glfwGetMonitors(&s_ember_monitor_count_);

    return s_ember_monitor_count_;
}

GLFWmonitor** EMBER_GetAvailableMonitors() {

    GLFWmonitor** monitors = glfwGetMonitors(&s_ember_monitor_count_);

    if (!monitors) {
        printf("ERROR: Failed to set monitor! No display attached.\n");
        abort();
    }

    return monitors;
}


// VSync

void EMBER_SetVSync(bool vsync) {
    s_ember_vsync_ = vsync;

    glfwSwapInterval((int) vsync);
}

bool EMBER_GetVSync() {
    return s_ember_vsync_;
}


// Refresh Rate

void EMBER_SetRefreshRate(int32_t fps) {
    const GLFWvidmode* video_mode = glfwGetVideoMode(s_ember_monitor_);

    if (fps <= 0) {
        s_ember_refresh_rate_ = video_mode->refreshRate;
    } else {
        s_ember_refresh_rate_ = fps;
    }
}

uint32_t EMBER_GetRefreshRate() {
    return s_ember_refresh_rate_;
}


// Retina Mode

void EMBER_SetRetinaMode(bool retina) {
    s_ember_retina_ = retina;
}

bool EMBER_GetRetinaMode() {
    return s_ember_retina_;
}


// GL Viewport

void EMBER_SetGLViewport(int32_t x, int32_t y, int32_t w, int32_t h) {
    if (s_ember_retina_) {
        glViewport(x, y, w * 2, h * 2);
    } else {
        glViewport(x, y, w, h);
    }
}