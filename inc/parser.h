#pragma once

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#define PARSER_MSG_NOT_FOUND -1
#define PARSER_SUCCESS 0

// container used for holding string type
typedef struct parser_string_t {
  char *data;
  size_t size;
} parser_string_t;

// struct for holding all the node information
typedef struct parser_node_t {
  struct parser_node_t *node;
  size_t size;
  parser_string_t message;
  int value;
} parser_node_t;

// container used for holding root node
typedef struct parser_t {
  struct parser_node_t *node;
  size_t size;
} parser_t;

// creating and freeing parser_parse tree
void parser_init(parser_t **tree);
void parser_free(parser_t *tree);

// add new word to parser
void parser_add(const char *name, int const value, parser_t *tree);

// return value for name, PARSER_MSG_NOT_FOUND if not found
int parser_parse(char const *name, parser_t *tree);

#ifdef __cplusplus
}
#endif
