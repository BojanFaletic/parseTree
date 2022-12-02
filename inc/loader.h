#pragma once
#include "list.h"
#include "parser.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef list_holder_t json_obj_t;
void parser_load_json(const char *f_name, parser_t *obj, json_obj_t **json);
void parser_free_json(json_obj_t *json);

#ifdef __cplusplus
}
#endif
