#include "engine.h"

#include "window.h"


// Static

/* This variable leaks memory, this is intentional */
static String s_ember_working_directory_;

static const char* s_ember_config_path_ = "config.yaml";


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

    return true;
}