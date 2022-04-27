#pragma once

#include "common.h"


// Refresh Rate
#define EMBER_NATIVE_RESRESH_RATE -1


// Window modes
#define EMBER_WINDOW_MODE_WINDOWED              0
#define EMBER_WINDOW_MODE_FULLSCREEN            1
#define EMBER_WINDOW_MODE_WINDOWED_FULLSCREEN   2


// Configurations

/* Sets the window properties from config file */
void EMBER_InitWindowProperties();


// Initialization

/* Initializes GLFW & GLEW and creates a window with the properties from config file */
bool EMBER_InitWindow();


// Window

/* Returns the glfw window */
GLFWwindow* EMBER_GLFWwindow();

bool EMBER_WindowShouldClose();

void EMBER_SetWindowShouldClose(bool value);

void EMBER_SwapBuffers();

void EMBER_ClearColor(float r, float g, float b);

void EMBER_Clear(uint32_t flags);


// Apply

/* Applies the latest changes to the window.
   This function just calls the 'EMBER_SetWindowMode' with currently active window mode. */
void EMBER_ApplyWindowSettings();


// Window Mode

/* Sets the window mode, and applies the latest changes to the window */
void EMBER_SetWindowMode(int32_t mode);

/* Returns the currently active window mode */
int32_t EMBER_GetWindowMode();


// Monitor

/* Sets the active monitor, user needs to apply changes to the window for this to affect */
void EMBER_SetMonitor(uint32_t monitor);

/* Returns the active monitor */
GLFWmonitor* EMBER_GetMonitor();

/* Returns the index of currently active monitor */
int32_t EMBER_GetMonitorIndex();

/* Returns the available monitor count, returns 0 if there is none or an error occured. */
int32_t EMBER_GetMonitorCount();

/* Returns the available monitors, if no monitor is attached or an error occured, aborts the program. 
   If you don't want to abort check the monitor count first. */
GLFWmonitor** EMBER_GetAvailableMonitors();


// VSync

void EMBER_SetVSync(bool vsync);

bool EMBER_GetVSync();


// Refresh Rate

/* Sets the refresh rate for the application.
   Pass EMBER_NATIVE_REFRESH_RATE for active monitor's default refresh rate */
void EMBER_SetRefreshRate(int32_t fps);

int32_t EMBER_GetRefreshRate();


// Retina Mode

/* Sets the retina mode, this is generaly used for MacOS devices,
   Don't change this if you're not sure you're building for a retina display machine. */
void EMBER_SetRetinaMode(bool retina);

bool EMBER_GetRetinaMode();


// GL Viewport

/* Sets the gl viewport, width and height is doubled if retina flag is set */
void EMBER_SetGLViewport(int32_t x, int32_t y, int32_t w, int32_t h);
