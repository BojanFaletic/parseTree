#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "list.h"
#include "parser.h"

#define MIN(A, B) ((A < B) ? A : B)

void print_node(node_t *nd);

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

void print_list(list_holder_t *list) {
  while (list->next != NULL) {
    node_t *nd = list->data;
    printf("List msg: %s\n", (char *)nd->message.data);
  }
}

void free_v3(parser_t *tree) {
  list_holder_t *list = list_init();
  size_t N = tree->size;

  if (N == 0) {
    free(tree);
    return;
  }

  node_t *tmp = tree->node;
  bool end = false;
  while (!end) {
    end = true;
    for (size_t i = 0; i < N; i++) {
      node_t *nd = &tmp[i];
      if (nd->size != 0) {
        tmp = nd->node;
        N = nd->size;
        end = false;
        break;
      }
    }
  }

  print_list(list);

  list_free(list);

  printf("-Freeing: %s\n", tmp->message.data);
  free(tmp);
  printf("-Freeing: tree\n");

  free(tree);
}

bool free_v2(parser_t **tree) {
  if (tree == NULL) {
    return false;
  }
  node_t *parent = (*tree)->node;
  if (parent == NULL) {
    free(*tree);
    *tree = NULL;
    printf("-Tree\n");
    return false;
  }

  node_t *child = parent->node;

  if (child == NULL) {
    printf("-Freeing: %s\n", parent->message.data);
    free(parent);
    printf("-Tree\n");

    free(*tree);
    *tree = 0;

    return false;
  }

  bool found = false;
start_search:
  for (size_t n = 0; n < child->size; n++) {
    node_t *tmp = child->node + n;
    if (tmp != NULL) {
      parent = child;
      child = tmp;
      found = true;
      goto start_search;
    }
  }
  printf("-Freeing: %s\n", child->message.data);

  free(child);
  parent->node = 0;
  return found;
}

static node_t *find_end_node(parser_t *tree) {
  node_t *end = tree->node;
  if (tree->size == 0 || end == NULL) {
    return NULL;
  }

start_search:
  for (size_t n = 0; n < end->size; n++) {
    if (&end->node[n] != NULL) {
      end = &end->node[n];
      goto start_search;
    }
  }
  return NULL;
}

void free_tree(parser_t *tree) {
  free_v3(tree);
  /*
  while (free_v2(&tree)) {
    printf(". ");
  }
  printf(" \n");
  */
}

parser_t *init_tree() {
  parser_t *tree = (parser_t *)malloc(sizeof(parser_t));
  *tree = (parser_t){.node = 0, .size = 0};
  return tree;
}

void link_node(node_t *parent, node_t *child) {
  node_t *current_nodes = parent->node;
  size_t const N = parent->size;
  size_t const N_new = N + 1;

  node_t *new_nodes = (node_t *)malloc(sizeof(node_t) * N_new);
  for (size_t i = 0; i < N; i++) {
    new_nodes[i] = current_nodes[i];
  }
  new_nodes[N] = *child;

  parent->node = new_nodes;
  parent->size = N_new;
  free(current_nodes);
}

void add_node(const char *name, int const value, node_t *node) {
  node_t child = {
    .node = NULL,
    .size = 0,
    .message= (string_t){.data=(char*)name, .size=strlen(name)},
    .value = value
    };

  link_node(node, &child);
  printf("+Adding: %s\n", name);
}

node_t *get_end_node(char **name, parser_t *tree) {
  // No previous node exist need to make new
  if (tree->size == 0) {
    return NULL;
  }

  string_t string = {.data = (char *)*name, .size = strlen(*name)};
  node_t *tmp = tree->node;
  node_t *selected_nd = tmp;
  bool keep_searching = true;

  while (keep_searching) {
    keep_searching = false;
    for (size_t n = 0; n < tmp->size; n++) {
      selected_nd = tmp->node + n;
      if (is_valid_node(&string, selected_nd)) {
        string.size -= selected_nd->message.size;
        string.data += selected_nd->message.size;

        tmp = selected_nd;
        keep_searching = true;
        break;
      }
    }
  }
  *name = string.data;
  return selected_nd;
}

void print_node(node_t *nd) {
  size_t non_zero_sz = 0;
  for (size_t i = 0; i < nd->size; i++) {
    if (&nd->node[i] != NULL) {
      non_zero_sz++;
    }
  }
  printf("Nd: %s, value: %d, next sz: %zu, alloc sz: %zu\n", nd->message.data,
         nd->value, nd->size, non_zero_sz);
}

size_t n_common_letters(char *const name, node_t const *nd) {
  size_t const max_search = MIN(strlen(name), nd->message.size);
  size_t i;

  for (i = 0; i < max_search; i++) {
    if (name[i] != nd->message.data[i]) {
      return i;
    }
  }
  return i;
}

void add_word(const char *name, int const value, parser_t *tree) {
  char *part_name = (char *)name;
  node_t *end_node = get_end_node(&part_name, tree);
  if (end_node == NULL) {
    printf("+Adding: %s\n", name);
    node_t *nd = (node_t *)malloc(sizeof(node_t));
    nd->message = (string_t){.data = (char *)name, .size = strlen(name)};
    nd->node = NULL;
    nd->size = 0;
    nd->value = value;

    tree->node = nd;
    tree->size = 1;
    return;
  }

  // find candidate node
  node_t *next_node = end_node;

  size_t n_same_letters = 0;
  node_t *candidate;
  size_t n_th_node = 0;
  for (n_th_node = 0; n_th_node < next_node->size; n_th_node++) {
    candidate = &next_node->node[n_th_node];

    // find same letters in node and current node
    n_same_letters = n_common_letters(part_name, candidate);
    if (n_same_letters != 0) {
      break;
    }
  }
  return;

  // merge current + candidate into new node
  if (n_same_letters != 0) {
    candidate->message.data += n_same_letters;
    candidate->message.size -= n_same_letters;
  }

  // add new intermediate node
  const char *new_node_msg = part_name + n_same_letters;
  add_node(new_node_msg, MSG_NOT_FOUND, end_node);

  // update candidate node to new value

  // add new node

  printf("Part name: %s\n", part_name);
  // print_node(end_node);
}

static int parse_recursive(string_t *string, node_t *node) {
#if DEBUG2
  printf("MSG: %s Nd: %s M_sz: %zu Nd_sz: %zu\n", string->data,
         node->message.data, string->size, node->message.size);
#endif
  if (is_valid_node(string, node)) {
    if (string->size == node->message.size) {
      return node->value;
    }
    string->data += node->message.size;
    string->size -= node->message.size;

    for (size_t n = 0; n < node->size; n++) {
      int value = parse_recursive(string, node->node + n);
      if (value != MSG_NOT_FOUND) {
        return value;
      }
    }
  }
  return MSG_NOT_FOUND;
}

int parse(char const *name, parser_t *tree) {
  string_t string = {.data = (char *)name, .size = strlen(name)};
  for (size_t n = 0; n < tree->size; n++) {
    int status = parse_recursive(&string, tree->node + n);
    if (status != MSG_NOT_FOUND) {
      return status;
    }
  }
  return MSG_NOT_FOUND;
}
