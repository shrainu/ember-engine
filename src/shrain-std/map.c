#include "map.h"

Map map_new() {
    const uint32_t default_size = 2;

    Map m = (Map) {
        .keys_   = (char**) calloc(default_size, sizeof(char*)),
        .values_ = (char**) calloc(default_size, sizeof(char*)),

        .count_ = 0,
        .capacity_ = 2,
    };

    return m;
}

void map_free(Map* map) {
    for (uint32_t i = 0; i < map->count_; ++i) {
        free(map->keys_[i]);
        free(map->values_[i]);
    }

    free(map->keys_);
    free(map->values_);

    map->count_ = 0;
    map->capacity_ = 0;
}

const char* map_get_key(Map* map, const char* key) {
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

ValuePair map_get_key_pair(Map* map, const char* key) {
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

void map_set_key(Map* map, const char* key, const char* value) {
    ValuePair pair = map_get_key_pair(map, key);
    
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