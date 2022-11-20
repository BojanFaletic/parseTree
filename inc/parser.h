#pragma once

#include <stddef.h>
#include <stdint.h>

#define MSG_NOT_FOUND -1
#define SUCCESS 0

// container used for holding string type
typedef struct string_t {
  char *data;
  size_t size;
} string_t;

// struct for holding all the node information
typedef struct node_t {
  struct node_t *node;
  size_t size;
  string_t message;
  int value;
} node_t;

// container used for holding root node
typedef struct parser_t{
  struct node_t *node;
  size_t size;
} parser_t;


// creating and freeing parser_parse tree
void parser_init(parser_t **tree);
void parser_free(parser_t *tree);

// add new word to parser
void parser_add(const char *name, int const value, parser_t *tree);

// return value for name, MSG_NOT_FOUND if not found
int parser_parse(char const *name, parser_t *tree);
