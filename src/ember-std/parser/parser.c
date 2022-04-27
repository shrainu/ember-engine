#include "parser.h"


Map* EMBER_ParseYaml(const char* path) {
    char* file = read_file(path);
    if (!file) {
        printf("ERROR: File could not be opened!\n");
        return NULL;
    }

    Map map = MAP_NEW();

    char* crnt = file;
    String line_string;
    int32_t line_number = -1;

    while ((line_string = EMBER_GetLine(crnt)).lenght != 0) {

        // Backup string
        String backup = STRING_FROM(line_string);

        // Advance the file pointer & line number
        crnt += line_string.lenght + 1;
        line_number++;

        // Trim the whitespace from the string
        STRING_TRIM_WHITESPACE(&line_string);

        // Remove the comments
        int32_t comment = STRING_FIND_FIRST(line_string, "#");
        if (comment != -1) {
            STRING_REMOVE(&line_string, comment, -1);
            if (line_string.buffer[comment - 1] == ' ') {
                STRING_REMOVE(&line_string, comment - 1, comment - 1);
            }
        }

        // Continue if the line is empty
        if (line_string.lenght == 0) {

            STRING_FREE(&line_string);
            STRING_FREE(&backup);
            continue;
        }

        // Find the divider
        int32_t divider = STRING_FIND_FIRST(line_string, ":");
        if (divider == -1) {
            printf("ERROR: %s:%d - Missing ':' at the end of the line\n", path, line_number);
            break;
        }

        // Find the points
        int32_t token_point = (line_string.buffer[divider - 1] == ' ') ? divider - 2 : divider - 1;
        if (divider == 0) {
            printf("ERROR: %s:%d - Missing token at the start of the line.\n", path, line_number);
            break;
        } else if (line_string.buffer[divider - 1] == ' ') {
            token_point = divider - 2;
        } else {
            token_point = divider - 1;
        }

        int32_t value_point = -1;
        if (divider == line_string.lenght - 1) {
            value_point = -1;
        } else if (line_string.buffer[divider + 1] == ' ') {
            value_point = divider + 2;
        } else {
            value_point = divider + 1;
        }

        // Parse the token
        String token = STRING_SUBSTRING(line_string, -1, token_point);

        if (STRING_FIND_FIRST(token, " ") != -1) {
            printf("ERROR: %s:%d - Token name can't contain any spaces.\n", path, line_number);
            break;
        }

        if (STRING_FIND_FIRST(token, "\"") != -1) {
            printf("ERROR: %s:%d - Token can't be a string, remove '\"' from the token name.\n", path, line_number);
            break;
        }

        // Parse the value
        String value;
        if (value_point == -1) {
            value = STRING_NEW("");
        } else {
            value = STRING_SUBSTRING(line_string, value_point, -1);
        }

        // Find out if value is a string
        bool str_value = false; 

        if (value.buffer[0] == '"' && value.buffer[value.lenght - 1] == '"') {
            str_value = true;
        } else if (STRING_FIND_FIRST(value, "\"") != -1) {
            printf("ERROR: %s:%d - Missing '\"' at the end of the line.\n", path, line_number);
            break;
        }

        // If value is not a string remove whitespaces, if it is get it from original line
        if (!str_value) {
            int32_t empty = STRING_FIND_FIRST(value, " ");
            if (empty != -1 && empty != value.lenght - 1) {
                printf("ERROR: %s:%d - Unknown type for token value.\n", path, line_number);
                break;
            }
        } else {
            STRING_FREE(&value);

            int32_t str_start = STRING_FIND_FIRST(backup, "\"");
            int32_t str_final = STRING_FIND_LAST(backup,  "\"");

            value = STRING_SUBSTRING(backup, str_start + 1, str_final - 1);
        }

        // Set the token and value
        MAP_SET_KEY(&map, token.buffer, value.buffer);

        // Cleanup
        STRING_FREE(&token);
        STRING_FREE(&value);

        STRING_FREE(&backup);
        STRING_FREE(&line_string);
    }

    // Free the file
    free(file);
    
    // Return the map
    Map* m = (Map*) malloc(sizeof(Map));
    *m = map;

    return m;
}