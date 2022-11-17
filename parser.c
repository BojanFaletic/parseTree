#include <stdbool.h>
#if DEBUG
#include <stdio.h>
#endif
#include <stdlib.h>
#include <string.h>

#include "parser.h"

static void free_tree_recursive(node_t *node) {
  if (node->size == 0) {
#if DEBUG
    printf("Free node: %s \n", node->message.data);
#endif
    free(node);
    return;
  }
  for (size_t i = 0; i < node->size; i++) {
    free_tree_recursive(node->next[i]);
  }
}

void free_tree(root_node_t *tree) {
  for (size_t n = 0; n < tree->size; n++) {
    free_tree_recursive(tree->nodes[n]);
  }
#if DEBUG
  printf("Free node pointers\n");
#endif
  free(tree);
}

root_node_t *init_tree() {
  root_node_t *tree = (root_node_t *)malloc(sizeof(root_node_t));
  tree->nodes = 0;
  tree->size = 0;
  return tree;
}

void add_node(const char *name, int const value, node_t *node) {
  node_t **current_nodes = node->next;
  size_t const N = node->size;
  size_t const N_new = N + 1;

  // create new node
  node_t *new_node = (node_t *)malloc(sizeof(node_t));
  new_node->next = 0;
  new_node->size = 0;
  new_node->value = value;

  string_t message = {.data = (char *)name, .size = sizeof(name)};
  new_node->message = message;

  // new next buffer
  node_t **new_nodes = (node_t **)malloc(sizeof(node_t *) * N_new);

  for (size_t i = 0; i < N; i++) {
    new_nodes[i] = current_nodes[i];
  }
  new_nodes[N] = new_node;

  node->next = new_nodes;
  node->size = N_new;

  free(current_nodes);
}

node_t *get_end_node(const char *name, root_node_t *tree) { return 0; }

void add_word(const char *name, int const value, root_node_t *tree) {}

static bool string_compare(string_t const *name, string_t const *node_string) {
  if (name->size < node_string->size) {
    return false;
  }
  for (size_t i = 0; i < node_string->size; i++) {
    if (name->data[i] != node_string->data[i]) {
      return false;
    }
  }
  return true;
}

static int parse_recursive(string_t *string, node_t const *node) {
  if (string_compare(string, &node->message)) {
    if (string->size == node->message.size) {
      return node->value;
    }
    for (size_t n = 0; n < node->size; n++) {
      string->data += node->message.size;
      string->size -= node->message.size;
      int status = parse_recursive(string, node->next[n]);
      if (status != MSG_NOT_FOUND) {
        return status;
      }
    }
  }
  return MSG_NOT_FOUND;
}

int parse(char const *name, root_node_t *tree) {
  string_t string = {.data = (char *)name, .size = strlen(name)};
  for (size_t n = 0; n < tree->size; n++) {
    int status = parse_recursive(&string, tree->nodes[n]);
    if (status != MSG_NOT_FOUND) {
      return status;
    }
  }
  return MSG_NOT_FOUND;
}
