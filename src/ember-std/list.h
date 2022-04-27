#pragma once

#include "common.h"

// Decleration
#define LIST_DECLARE(type) \
    typedef struct list_##type##_t { \
        type* array; \
        uint32_t count; \
        uint32_t capacity; \
    } list_##type##_t

#define LIST_TYPE(type) list_##type##_t* 

#define LIST_PTR_DECLARE(type) \
    typedef struct list_##type##_ptr_t {\
        type** array; \
        uint32_t count; \
        uint32_t capacity; \
    } list_##type##_ptr_t

#define LIST_PTR_TYPE(type) list_##type##_ptr_t*

// Initialization & termination
#define LIST_NEW(ptr, type) malloc(sizeof(list_##type##_t)); \
    *ptr = (list_##type##_t) { \
        .array = (type*) malloc(sizeof(type) * 2), \
        .count = 0, \
        .capacity = 2 \
    }

#define LIST_PTR_NEW(ptr, type) malloc(sizeof(list_##type##_ptr_t)); \
    *ptr = (list_##type##_ptr_t) { \
        .array = (type**) malloc(sizeof(type*) * 2), \
        .count = 0, \
        .capacity = 2 \
    }

#define LIST_FREE(list) \
    free(list)

// Functionality
#define LIST_GET(list, index) \
    list->array[index]

#define LIST_PUSH(list, item) \
    if (list->count == list->capacity) { \
        list->capacity *= 2; \
        list->array = (typeof(list->array[0])*) realloc(list->array, list->capacity * sizeof(list->array[0])); \
    } \
    list->array[list->count++] = item

#define LIST_REMOVE(list, index) \
    if (index != list->count - 1) { \
        typeof(list->array[0]) *ptr = list->array + index; \
        memmove(ptr, ptr + 1, (list->count - (index + 1)) * sizeof(list->array[0]));\
    } \
    list->count--

#define LIST_CLEAR(list) \
    list->count = 0