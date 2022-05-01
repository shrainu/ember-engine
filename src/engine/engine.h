#pragma once

#include "common.h"


// OpenGL Debugging

#define EMBER_GL_CLEAR_ERROR() \
   while(glGetError() != GL_NO_ERROR);

#define EMBER_GL_CHECK_ERROR(func, args...) \
   { \
      func(args); \
      uint32_t err; \
      if ((err = glGetError()) != GL_NO_ERROR) { \
         const char* msg = gluErrorString(err); \
         printf("ERROR: @%s %s:%d - %d.\n", #func, __FILE__, __LINE__, err); \
      } \
   }


// Performance

/* Calculates the delta time and the FPS */
void EMBER_CalculateFrametime();

/* Returns the delta time */
double EMBER_GetDeltaTime();

/* Returns the FPS */
uint32_t EMBER_GetFPS();


// Working Directory

/* This function should not be called outside of the engine */
void EMBER_SetWorkingDirectory(const char* path);

/* Returns the current working directory of the application */
const String* EMBER_GetWorkingDirectory();

/* Appends given path to current working directory, and retuns the string.
   User has to free the memory allocated for the strings buffer. */
String EMBER_GetRelativePath(const char* path);


// Config Path

/* Returns the path for apps configuration file.
   User has to free the memory allocated for the strings buffer. */
String EMBER_GetConfigPath();


// Initialization

/* Initializes the whole Ember Engine */
bool EMBER_InitEngine(int argc, char* argv[]);

/* Terminates the whole Ember Engine */
void EMBER_TerminateEngine();