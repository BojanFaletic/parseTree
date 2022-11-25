#pragma once
#include "list.h"
#include "parser.h"

typedef list_holder_t json_obj_t;
void parser_load_json(const char *f_name, parser_t *obj, json_obj_t **json);
void parser_free_json(json_obj_t *json);
