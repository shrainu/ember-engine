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

static Map MAP_NEW() {
    const uint32_t default_size = 2;

    Map m = (Map) {
        .keys_   = (char**) calloc(default_size, sizeof(char*)),
        .values_ = (char**) calloc(default_size, sizeof(char*)),

        .count_ = 0,
        .capacity_ = 2,
    };

    return m;
}

static void MAP_FREE(Map* map) {
    for (uint32_t i = 0; i < map->count_; ++i) {
        free(map->keys_[i]);
        free(map->values_[i]);
    }

    free(map->keys_);
    free(map->values_);

    map->count_ = 0;
    map->capacity_ = 0;
}

static const char* MAP_GET_KEY(Map* map, const char* key) {
    char* value_ = NULL;

    for (uint32_t index = 0; index < map->count_; ++index) {
        char* key_ = map->keys_[index];
        
        if (strcmp(key_, key) == STR_EQUAL) {
            value_ = map->values_[index];
            break;
        }
    }
    
    return value_;
}

static ValuePair MAP_GET_KEY_PAIR(Map* map, const char* key) {
    char* value_ = NULL;
    uint32_t index_ = 0;

    for (uint32_t index = 0; index < map->count_; ++index) {
        char* key_ = map->keys_[index];
        
        if (strcmp(key_, key) == STR_EQUAL) {
            value_ = map->values_[index];
            index_ = index;
            break;
        }
    }

    return (ValuePair) { 
        .value = value_,
        .index = index_,
    };
}

static void MAP_SET_KEY(Map* map, const char* key, const char* value) {
    ValuePair pair = MAP_GET_KEY_PAIR(map, key);
    
    if (pair.value) {
        size_t len = strlen(value);
        if (len <= 0) {
            free(map->values_[pair.index]);
            
            map->values_[pair.index] = (char*) calloc(1, sizeof(char));
            return;
        } else {
            free(map->values_[pair.index]);

            char* str = (char*) calloc(len + 1, sizeof(char));
            map->values_[pair.index] = strcpy(str, value);
            return;
        }
    }

    if (map->count_ == map->capacity_) {
        map->capacity_ *= 2;
        uint32_t size = map->capacity_ * sizeof(char*);

        map->keys_   = (char**) realloc(map->keys_,   size);
        map->values_ = (char**) realloc(map->values_, size);
    }

    uint32_t current = map->count_++;

    size_t len = strlen(key);
    char* ckey = (char*) calloc(len + 1, sizeof(char));
    map->keys_[current] = strcpy(ckey, key);

    len = strlen(value);
    if (len <= 0) {
        map->values_[current] = (char*) calloc(1, sizeof(char));;
    } else {
        char* cvalue = (char*) calloc(len + 1, sizeof(char));
        map->values_[current] = strcpy(cvalue, value);
    }
}

static bool MAP_PARSE_TAG(Map* map, const char* key) {
    const char* value = MAP_GET_KEY(map, key);
    if (value == NULL) {
        printf("ERROR: Map doesn't contain the key '%s'.\n", key);
        return 0;
    }

    size_t len = strlen(value);
    if (len == 0) {
        return true;
    }

    return false;
}

static int MAP_PARSE_INT(Map* map, const char* key) {
    const char* value = MAP_GET_KEY(map, key);
    if (value == NULL) {
        printf("ERROR: Map doesn't contain the key '%s'.\n", key);
        return 0;
    }

    char* c = NULL;
    int result = (int) strtol(value, &c, 10);

    if (c && *c == '\0') {
        return result;
    }

    printf("ERROR: Failed to parse int from value '%s'.\n", value);
    return 0;
}

static String MAP_PARSE_STRING(Map* map, const char* key) {
    const char* value = MAP_GET_KEY(map, key);
    if (value == NULL) {
        printf("ERROR: Map doesn't contain the key '%s'.\n", key);
        return (String) {.buffer = NULL, .lenght = 0, .capacity = 0};
    }

    return STRING_NEW(value);
}

static bool MAP_PARSE_BOOL(Map* map, const char* key) {
    const char* value = MAP_GET_KEY(map, key);
    if (value == NULL) {
        printf("ERROR: Map doesn't contain the key '%s'.\n", key);
        return 0;
    }

    size_t len = strlen(value) + 1;
    if (len - 1 > 5) {
        printf("ERROR: Failed to parse bool from value '%s'.\n", value);
        return false;
    }

    if (strcmp(value, "true") == STR_EQUAL || (len - 1 == 1 && *value == '1')) {
        return true;
    } else if (strcmp(value, "false") == STR_EQUAL || (len - 1 == 1 && *value == '0')) {
        return false;
    }

    printf("ERROR: Failed to parse bool from value '%s'.\n", value);
    return false;
}