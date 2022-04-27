#pragma once

#include "common.h"


/* Parses a .yaml file at the given path. Caller has to free the map afterwards.
   This function leaks memory if it failes to parse the yaml file. */
Map* EMBER_ParseYaml(const char* path);