#include "parser.h"

// TODO: Fix potential memory leak
Map* parser_parse_yaml(const char* path) {
    char* file = read_file(path);
    if (!file) {
        return NULL;
    }

    Map map = map_new();

    char* crnt = file;
    char* line = NULL;
    while ((line = get_line(crnt)) != NULL) {

        size_t len = line - crnt + 1;
        char* str = (char*) calloc(len + 1, sizeof(char));
        char* ptr = str;

        bool start = true;
        bool in_word = false;

        for (char* c = crnt; c <= line; ++c) {

            char chr = *c;

            if (chr == '#') {
                break;
            }

            if (!in_word && chr != ' ' && chr != '\t') {
                in_word = true;

                if (!start) {
                    bool space = true;
                    if (c != line && *(c + 1) == '#') {
                        space = false;
                    }
                    if (space) {
                        *ptr = ' ';
                        ptr++;
                    }
                }
            } if (in_word && (chr == ' ' || chr == '\t')) {
                in_word = false;

                if (start) {
                    start = false;
                }
            }

            if (in_word) {
                if (c == line && chr == '\n') {
                    continue;
                }

                *ptr = chr;
                ptr++;
            }
        }

        size_t str_len = strlen(str);
        if (str_len <= 0) {
            free(str);
            crnt = line + 1;
            
            continue;
        }

        in_word = true;

        bool token = true;
        bool value = false;

        char* token_str = (char*) calloc(257, sizeof(char));
        char* token_ptr = token_str;

        char* value_str = (char*) calloc(257, sizeof(char));
        char* value_ptr = value_str;

        for (char* c = str; c < str + str_len; ++c) {

            char chr = *c;

            if (!in_word && (chr != ' ' || chr != '\t')) {
                in_word = true;
            } else if (in_word && (chr == ' ' || chr == '\t')) {
                in_word = false;
            }

            if (token && in_word) {
                if (chr == ':') {
                    token = false;
                    value = true;

                    if (c == str + str_len - 1) {
                        break;
                    }

                    continue;
                }

                if (token_ptr - token_str == 256) {
                    printf("SYNTAX: ERROR: Token name can't be longer than 256 characters.\n");

                    free(value_str);
                    free(token_str);
                    free(str);

                    return NULL;
                }

                *token_ptr = chr;
                token_ptr++;
            } else if (token && !in_word) {
                
                if (c == str + str_len - 1) {
                    printf("SYNTAX: ERROR: Token definition should end with a ':'.\n");

                    free(value_str);
                    free(token_str);
                    free(str);

                    return NULL;
                } else if (*(c + 1) != ':') {
                    printf("SYNTAX: ERROR: Token name can't contain ' '.\n");

                    free(value_str);
                    free(token_str);
                    free(str);

                    return NULL;
                }

                token = false;
                value = true;
            }

            if (value && in_word) {
                if (c == str + str_len - 1 && chr == ':') {
                    break;
                }
                if (chr == ':') {
                    continue;
                }

                if (value_ptr - value_str == 256) {
                    printf("SYNTAX: ERROR: Token value can't be longer than 256 characters.\n");

                    free(value_str);
                    free(token_str);
                    free(str);

                    return NULL;
                }

                *value_ptr = chr;
                value_ptr++;
            }
        }

        map_set_key(&map, token_str, value_str);

        free(str);
        free(value_str);
        free(token_str);

        crnt = line + 1;
    }

    free(file);
    
    Map* m = calloc(1, sizeof(Map));
    *m = map;

    return m;
}

bool parser_yaml_parse_tag(Map* map, const char* key) {
    const char* value = map_get_key(map, key);
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

int parser_yaml_parse_int(Map* map, const char* key) {
    const char* value = map_get_key(map, key);
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

char* parser_yaml_parse_str(Map* map, const char* key) {
    const char* value = map_get_key(map, key);
    if (value == NULL) {
        printf("ERROR: Map doesn't contain the key '%s'.\n", key);
        return 0;
    }

    size_t len = strlen(value) + 1;
    char* str = (char*) calloc(len, sizeof(char));

    return strcpy(str, value);
}

bool parser_yaml_parse_bool(Map* map, const char* key) {
    const char* value = map_get_key(map, key);
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