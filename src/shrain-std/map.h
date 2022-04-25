#pragma once

#include "common.h"
#include "util.h"

typedef struct Map {
    char** keys_;
    char** values_;

    uint32_t count_;
    uint32_t capacity_;
} Map;

typedef struct ValuePair {
    char* value;
    uint32_t index;
} ValuePair;

Map map_new();

void map_free(Map* map);

const char* map_get_key(Map* map, const char* key);

void map_set_key(Map* map, const char* key, const char* value);