#pragma once

#include "common.h"

// Type
typedef struct String {
    char* buffer;
    size_t lenght;
    size_t capacity;
} String;

// Debug
#define STRING_DEBUG_PRINT(string) \
    printf("%s = (String) { `%s`, %lu, %lu }\n", #string, string.buffer, string.lenght, string.capacity)

// Constructor & Destructor
static String STRING_NEW(const char* str) {
    size_t len = strlen(str);

    String string = (String) {
        .buffer = (char*) calloc(len + 1, sizeof(char)),
        .lenght = len,
        .capacity = len + 1
    };

    strcpy(string.buffer, str);

    return string;
}

static String STRING_FROM(String string) {

    return STRING_NEW(string.buffer);
}

static String STRING_SUBSTRING(String string, int32_t begin, int32_t end) {

    begin = (begin < 0) ? 0 : begin;
    end   = (end   < 0) ? string.lenght - 1 : end;

    if (begin > end) {
        printf("ERROR: Couldn't get substring from string! Begin can't be bigger than end.\n");
        abort();
    }

    char temp = string.buffer[end + 1];
    string.buffer[end + 1] = '\0';

    String new = STRING_NEW(string.buffer + begin);

    string.buffer[end + 1] = temp;

    return new;
}

static void STRING_FREE(String* string) {

    if (string->capacity > 0) {
        free(string->buffer);
    }

    string->buffer = NULL;
    string->capacity = 0;
    string->lenght = 0;
}

// Methods
static const char* STRING_GET(String string) {
    
    return (const char*) string.buffer;
}

static String STRING_COPY(String* string, const char* str) {

    size_t len = strlen(str);

    if (string->capacity <= len) {
        string->capacity = len + 1;
        
        free(string->buffer);
        string->buffer = (char*) calloc(string->capacity, sizeof(char));
    }

    strcpy(string->buffer, str);

    string->buffer[len] = '\0';
    string->lenght = len;
    
    return *string;
}

static String STRING_APPEND(String* string, const char* str) {

    size_t len = string->lenght + strlen(str);

    if (string->capacity <= len) {
        string->capacity = len + 1;
        string->buffer = (char*) realloc(string->buffer, sizeof(char) * string->capacity);
    }

    strcpy(string->buffer + string->lenght, str);

    string->buffer[len] = '\0';
    string->lenght = len;

    return *string;
}

static String STRING_PUSH(String* string, char c) {

    if (string->lenght == string->capacity - 1) {
        string->capacity *= 2;
        string->buffer = (char*) realloc(string->buffer, sizeof(char) * string->capacity);
    }

    string->buffer[string->lenght++] = c;
    string->buffer[string->lenght] = '\0';

    return *string;
}

static String STRING_REMOVE(String* string, int32_t begin, int32_t end) {
    
    begin = (begin < 0) ? 0 : begin;
    end   = (end   < 0) ? string->lenght - 1 : end;

    if (begin > end) {
        printf("ERROR: Couldn't remove from string! Begin can't be bigger than end.\n");
        abort();
    }

    if (end >= string->lenght - 1) {
        string->buffer[begin] = '\0';
        string->lenght = begin;

        return *string;
    }

    char* begin_ptr = string->buffer + begin;
    char* move_ptr  = string->buffer + end + 1;

    size_t move_size = (string->capacity - end) * sizeof(char);
    size_t removed = end - begin + 1;

    memmove(begin_ptr, move_ptr, move_size);
    string->lenght -= removed;

    return *string;
}

static int32_t STRING_FIND_LAST(String string, const char* str)  {

    size_t len = strlen(str);

    int32_t index = -1;

    for (uint32_t i = 0; i <= string.lenght - len; ++i) {

        int32_t result = strncmp(string.buffer + i, str, sizeof(char) * len);

        if (result == 0) {
            index = i;
        }
    }

    return index;
}

static int32_t STRING_FIND_FIRST(String string, const char* str)  {

    size_t len = strlen(str);

    for (uint32_t i = 0; i <= string.lenght - len; ++i) {

        int32_t result = strncmp(string.buffer + i, str, sizeof(char) * len);

        if (result == 0) {
            return i;
        }
    }

    return -1;
}
