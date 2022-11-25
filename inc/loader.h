#pragma once
#include "list.h"
#include "parser.h"

void load_json(const char *f_name, parser_t *obj, list_holder_t **json);
void json_free(list_holder_t *json);
