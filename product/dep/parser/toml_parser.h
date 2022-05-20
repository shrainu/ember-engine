#pragma once

/*
 * TOML PARSER v0.4
 * This is a toml parser written in Rust.
 *
 * NOTES:
 * Don't free any memory this library allocates.
 * Use provided functions to free the memory.
 */

#ifdef __cplusplus
extern "C" {
#endif

// Includes
#include <inttypes.h>


// Declaration of the original Rust struct
typedef struct TOMLStringMap TOMLStringMap;


// Parses TOML file into a map
void toml_parser_parse(TOMLStringMap** map, const char* filepath);

// Frees the memory Rust's allocated for the map
void toml_parser_free(TOMLStringMap* map);


// Cleanup
void toml_parser_free_string(char* string);

void toml_parser_free_int_array(int32_t* array);

void toml_parser_free_int_array_array(int32_t** array, int32_t size);


// Gets
int32_t toml_parser_get_int(TOMLStringMap* map, const char* tag, const char* name);

char* toml_parser_get_string(TOMLStringMap* map, const char* tag, const char* name);

bool toml_parser_get_bool(TOMLStringMap* map, const char* tag, const char* name);

int32_t* toml_parser_get_int_array(TOMLStringMap* map, const char* tag, const char* name);

int32_t** toml_parser_get_int_array_array(TOMLStringMap* map, const char* tag, const char* name, int32_t* size);

#ifdef __cplusplus
}
#endif