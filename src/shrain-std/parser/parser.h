#pragma once

#include "common.h"

Map* parser_parse_yaml(const char* path);

bool parser_yaml_parse_tag(Map* map, const char* key);

int parser_yaml_parse_int(Map* map, const char* key);

char* parser_yaml_parse_str(Map* map, const char* key);

bool parser_yaml_parse_bool(Map* map, const char* key);
