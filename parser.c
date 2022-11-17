#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "parser.h"

static void free_tree_recursive(node_t *node) {
  if (node == NULL) {
    return;
  }

  if (node->next.node != NULL) {
    free(node->next.node);
    return;
  }

  for (size_t n = 0; n < node->next.size; n++) {
    free_tree_recursive(node->next.node + n);
  }
}

void free_tree(parser_t *tree) {
  free_tree_recursive(tree->next.node);
  free(tree);
}

parser_t *init_tree() {
  parser_t *tree = (parser_t *)malloc(sizeof(parser_t));
  tree->next = (basic_node_t){.node = 0, .size=0};
  return tree;
}

void add_node(const char *name, int const value, node_t *node) {
  node_t *current_nodes = node->next.node;
  size_t const N = node->next.size;
  size_t const N_new = N + 1;

  // new next buffer
  node_t *new_nodes = (node_t *)malloc(sizeof(node_t) * N_new);
  memcpy(new_nodes, current_nodes, sizeof(node_t) * N);

  new_nodes[N].message = (string_t){.data = (char *)name, .size = sizeof(name)};
  new_nodes[N].value = value;
  node->next.node = new_nodes;
  node->next.size = N_new;
  free(current_nodes);
}

node_t *get_end_node(const char *name, parser_t *tree) { return 0; }

void add_word(const char *name, int const value, parser_t *tree) {}

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

static int parse_recursive(string_t *string, node_t *node) {
  if (string_compare(string, &node->message)) {
    if (string->size == node->message.size) {
      return node->value;
    }
    for (size_t n = 0; n < node->next.size; n++) {
      string->data += node->message.size;
      string->size -= node->message.size;
      int value = parse_recursive(string, node->next.node + n);
      if (value != MSG_NOT_FOUND) {
        return value;
      }
    }
  }
  return MSG_NOT_FOUND;
}

int parse(char const *name, parser_t *tree) {
  string_t string = {.data = (char *)name, .size = strlen(name)};
  for (size_t n = 0; n < tree->next.size; n++) {
    int status = parse_recursive(&string, tree->next.node + n);
    if (status != MSG_NOT_FOUND) {
      return status;
    }
  }
  return MSG_NOT_FOUND;
}
