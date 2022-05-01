#include "engine.h"

#include "window.h"
#include "renderer.h"


// Static

/* This variable leaks memory, this is intentional */
static String s_ember_working_directory_;

static const char* s_ember_config_path_ = "config.yaml";

// Performance related variables

static const double S_EMBER_DELTA_TIME_HIGH = 0.1;

static double s_ember_delta_time_last_;
static double s_ember_delta_time_;

static double s_ember_fps_last_time_;
static uint32_t s_ember_fps_counter_;
static uint32_t s_ember_fps_;


// Performance

void EMBER_CalculateFrametime() {

    // Calculate delta time
    double delta_time_current = glfwGetTime();

    s_ember_delta_time_ = delta_time_current - s_ember_delta_time_last_;
    if (s_ember_delta_time_ >= S_EMBER_DELTA_TIME_HIGH) {
        s_ember_delta_time_ = S_EMBER_DELTA_TIME_HIGH;
    }

    s_ember_delta_time_last_ = delta_time_current;

    // Calculate fps
    double time_diff = delta_time_current - s_ember_fps_last_time_;
    s_ember_fps_counter_++;

    if (time_diff >= 1 / 30.0) {
        s_ember_fps_ = (1 / time_diff) * s_ember_fps_counter_;
        
        s_ember_fps_last_time_ = delta_time_current;
        s_ember_fps_counter_ = 0;
    }
}

double EMBER_GetDeltaTime() {
    return s_ember_delta_time_;
}

uint32_t EMBER_GetFPS() {
    return s_ember_fps_;
}


// Working Directory

void EMBER_SetWorkingDirectory(const char* path) {

    s_ember_working_directory_ = STRING_NEW(path);

    int32_t index = STRING_FIND_LAST(s_ember_working_directory_, "/");

    if (index == -1) {
        printf("ERROR: Failed to figure the working directory.\n");
        abort();
    }
    
    STRING_REMOVE(&s_ember_working_directory_, index + 1, -1);
}

const String* EMBER_GetWorkingDirectory() {

    return (const String*) &s_ember_working_directory_;
}

String EMBER_GetRelativePath(const char* path) {
    
    String rel_path = STRING_FROM(s_ember_working_directory_);

    return STRING_APPEND(&rel_path, path);
}


// Config Path

String EMBER_GetConfigPath() {
    
    String path = STRING_FROM(s_ember_working_directory_);
    
    return STRING_APPEND(&path, s_ember_config_path_);
}


// Initialization

bool EMBER_InitEngine(int argc, char* argv[]) {

    // Set the working directory
    EMBER_SetWorkingDirectory(argv[0]);

    // Initialize the window
    if (!EMBER_InitWindow()) {
        return false;
    }

    // Initialize the renderer
    if (!EMBER_InitRenderer()) {
        return false;
    }

    return true;
}

void EMBER_TerminateEngine() {

    // Terminate the renderer
    EMBER_TerminateRenderer();

    // Terminate the window
    EMBER_TerminateWindow();
}