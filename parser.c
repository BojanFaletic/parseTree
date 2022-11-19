#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "parser.h"

static bool is_valid_node(string_t const *name, node_t const *node) {
  string_t const *node_v = &node->message;

  if (name->size < node_v->size) {
    return false;
  }
  for (size_t i = 0; i < node_v->size; i++) {
    if (name->data[i] != node_v->data[i]) {
      return false;
    }
  }
  return true;
}

static node_t *find_end_node(parser_t *tree) {
  basic_node_t *data_node = &tree->next;
  node_t *end = NULL;

  if (data_node->size == 0) {
    return NULL;
  }


start_search:
  for (size_t n = 0; n < data_node->size; n++) {
    node_t *pointer_node = &data_node->node[n];
    if (pointer_node != NULL) {
      *data_node = pointer_node->next;
      end = pointer_node;
      goto start_search;
    }
  }
  return end;
}

void free_tree(parser_t *tree) {
  while (true) {
    node_t *nd = find_end_node(tree);
    if (nd == NULL) {
      break;
    }
    free(nd);
    nd = 0;
  }

  free(tree);
}

parser_t *init_tree() {
  parser_t *tree = (parser_t *)malloc(sizeof(parser_t));
  tree->next = (basic_node_t){.node = 0, .size = 0};
  return tree;
}

void add_node(const char *name, int const value, node_t *node) {
  node_t *current_nodes = node->next.node;
  size_t const N = node->next.size;
  size_t const N_new = N + 1;

  // new next buffer
  node_t *new_nodes = (node_t *)malloc(sizeof(node_t) * N_new);
  memcpy(new_nodes, current_nodes, sizeof(node_t) * N);

  new_nodes[N].message = (string_t){.data = (char *)name, .size = strlen(name)};
  new_nodes[N].value = value;
  node->next.node = new_nodes;
  node->next.size = N_new;
  free(current_nodes);
}

node_t *get_end_node(const char *name, parser_t *tree) {
  string_t string = {.data = (char *)name, .size = strlen(name)};
  basic_node_t *tmp = &tree->next;
  node_t *selected_nd = 0;

  // No previus node exist need to make new
  if (tree->next.size == 0) {
    return selected_nd;
  }

  bool keep_searching = true;
  while (keep_searching) {
    keep_searching = false;
    for (size_t n = 0; n < tmp->size; n++) {
      selected_nd = tmp->node + n;
      if (is_valid_node(&string, selected_nd)) {
        string.size -= selected_nd->message.size;
        string.data += selected_nd->message.size;

        tmp = &selected_nd->next;
        keep_searching = true;
        break;
      }
    }
  }
  return selected_nd;
}

void add_word(const char *name, int const value, parser_t *tree) {
  node_t *end_node = get_end_node(name, tree);
  if (end_node == NULL) {
    printf("Adding node\n");

    node_t *nd = (node_t *)malloc(sizeof(node_t));
    nd->message = (string_t){.data = (char *)name, .size = strlen(name)};
    nd->next = (basic_node_t){.node = 0, .size = 0};
    nd->value = value;

    tree->next = (basic_node_t){.node = nd, .size = 1};
    return;
  }
}

static int parse_recursive(string_t *string, node_t *node) {
#if DEBUG
  printf("MSG: %s Nd: %s M_sz: %zu Nd_sz: %zu\n", string->data,
         node->message.data, string->size, node->message.size);
#endif
  if (is_valid_node(string, node)) {
    if (string->size == node->message.size) {
      return node->value;
    }
    string->data += node->message.size;
    string->size -= node->message.size;

    for (size_t n = 0; n < node->next.size; n++) {
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
