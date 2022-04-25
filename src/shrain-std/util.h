#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>
#include <memory.h>
#include <inttypes.h>

// Utility Macros
#define STR_EQUAL 0
#define FILE_READ_AT_ONCE 6400

// Utility Functions

// Caller has to free this memory if its not NULL
static char* read_file(const char* path) {
    FILE* file = NULL;
    if (!(file = fopen(path, "r"))) {
        printf("ERROR: Could not open file at '%s'.\n", path);
        return NULL;
    }

    size_t buffer_size = 0;
    size_t buffer_cap  = FILE_READ_AT_ONCE;

    char* buffer = (char*) calloc(buffer_cap, sizeof(char));
    char* buffer_ptr = buffer;

    size_t read;
    while ((read = fread(buffer_ptr, sizeof(char), FILE_READ_AT_ONCE, file)) > 0) {

        size_t left = FILE_READ_AT_ONCE - read;
        buffer_size += read;

        if (left == 0) {
            buffer_cap += FILE_READ_AT_ONCE;
        } else {
            buffer_cap -= left;
            break;
        }

        buffer = (char*) realloc(buffer, sizeof(char) * buffer_cap);
        buffer_ptr = buffer + buffer_size;
    }

    buffer = (char*) realloc(buffer, sizeof(char) * buffer_cap + 1);
    buffer[buffer_cap] = '\0';

    fclose(file);

    return buffer;
}

static char* get_line(char* file) {
    size_t len_ = strlen(file);
    if (len_ == 0) {
        return NULL;
    }

    char* line = NULL;
    for (char* c = file; c < file + len_; ++c) {
        if (*c == '\n' || c == file + len_ - 1) {
            line = c;
            break;
        }
    }

    return line;
}