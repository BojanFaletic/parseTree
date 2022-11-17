#pragma once

#include <stddef.h>
#include <stdint.h>

#define MSG_NOT_FOUND -1
#define SUCCESS 0

typedef struct string_t {
  char *data;
  size_t size;
} string_t;

typedef struct node_t {
  struct node_t **next;
  size_t size;
  string_t message;
  int value;
} node_t;

typedef struct root_node_t {
  struct node_t **nodes;
  size_t size;
} root_node_t;

root_node_t *init_tree();
int parse(char const *name, root_node_t *tree);
void free_tree(root_node_t *tree);

// temp
void add_node(const char *name, int const value, node_t *node);