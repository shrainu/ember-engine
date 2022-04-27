#pragma once

#include "common.h"


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