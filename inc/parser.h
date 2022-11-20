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


parser_t *init_tree();
int parse(char const *name, parser_t *tree);
void free_tree(parser_t *tree);

// temp
void add_node(const char *name, int const value, node_t **node);
void add_word(const char *name, int const value, parser_t *tree);
node_t *get_end_node(char **name, parser_t *tree);