#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "list.h"
#include "parser.h"

#ifdef __cplusplus
extern "C" {
#endif

#define MIN(A, B) ((A < B) ? A : B)

void print_node(parser_node_t *nd);

static bool is_valid_node(parser_string_t *name, parser_node_t *node) {
  parser_string_t const *node_v = &node->message;

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
  if (list == NULL) {
    return;
  }

  while (list->next != NULL) {
    parser_node_t *nd = list->data;
    if (nd == NULL) {
      printf("Node corruption data is NULL\n");
      return;
    }

    list = list->next;
    printf("List msg: %s\n", (char *)nd->message.data);
  }

  parser_node_t *nd1 = list->data;
  if (nd1 == NULL) {
    return;
  }
  printf("List msg: %s\n", (char *)nd1->message.data);
}

void map_all_nodes(parser_node_t *nd, list_holder_t *list) {
  if (nd->size != 0) {
    list_append(nd->node, list);
#ifdef PARSER_DEBUG
    printf("Adding: %zu\n", nd->size);
#endif
  }

  for (size_t i = 0; i < nd->size; i++) {
    parser_node_t *tmp = &nd->node[i];
    map_all_nodes(tmp, list);
  }
}

void free_all_nodes_in_list(list_holder_t *list) {
  find_end(&list);
  while (list->prev != NULL) {
    parser_node_t *nd = list->data;
#ifdef PARSER_DEBUG
    printf("-Free: %s\n", nd->message.data);
#endif
    free(nd);
    list = list->prev;
  }

  // free last
  parser_node_t *nd2 = list->data;
#ifdef PARSER_DEBUG
  printf("-Free: %s\n", nd2->message.data);
#endif
  free(nd2);
}

void parser_free(parser_t *tree) {
  list_holder_t *list = list_init();

  // add first layer
  if (tree->size != 0) {
    list_append(tree->node, list);
  }

  // add first element of all next layers
  for (size_t i = 0; i < tree->size; i++) {
    map_all_nodes(tree->node + i, list);
  }
#ifdef PARSER_DEBUG
  print_list(list);
#endif
  free_all_nodes_in_list(list);
  list_free(list);
  free(tree);
}

void parser_init(parser_t **tree) {
  *tree = (parser_t *)calloc(sizeof(parser_t), 1);
}

void link_node(parser_node_t *parent, parser_node_t *child) {
  parser_node_t *current_nodes = parent->node;
  size_t const N = parent->size;
  size_t const N_new = N + 1;

  parser_node_t *new_nodes =
      (parser_node_t *)malloc(sizeof(parser_node_t) * N_new);
  for (size_t i = 0; i < N; i++) {
    new_nodes[i] = current_nodes[i];
  }
  new_nodes[N] = *child;

  parent->node = new_nodes;
  parent->size = N_new;
  free(current_nodes);
}

void link_root_node(parser_t *parent, parser_node_t *child) {
  parser_node_t *current_nodes = parent->node;
  size_t const N = parent->size;
  size_t const N_new = N + 1;

  parser_node_t *new_nodes =
      (parser_node_t *)malloc(sizeof(parser_node_t) * N_new);
  for (size_t i = 0; i < N; i++) {
    new_nodes[i] = current_nodes[i];
  }
  new_nodes[N] = *child;

  parent->node = new_nodes;
  parent->size = N_new;
  free(current_nodes);
}

void add_node(const char *name, int const value, parser_node_t *node) {
  parser_node_t child = {
      .node = NULL,
      .size = 0,
      .message = (parser_string_t){.data = (char *)name, .size = strlen(name)},
      .value = value};

  link_node(node, &child);
#ifdef PARSER_DEBUG
  printf("+Adding: %s\n", name);
#endif
}

void add_root_node(const char *name, int const value, parser_t *node) {
  parser_node_t child = {
      .node = NULL,
      .size = 0,
      .message = (parser_string_t){.data = (char *)name, .size = strlen(name)},
      .value = value};

  link_root_node(node, &child);
#ifdef PARSER_DEBUG
  printf("+Adding: %s\n", name);
#endif
}

size_t n_common_letters(const char *name, parser_node_t const *nd) {
  size_t const max_search = MIN(strlen(name), nd->message.size);
  size_t i;

  for (i = 0; i < max_search; i++) {
    if (name[i] != nd->message.data[i]) {
      return i;
    }
  }
  return i;
}

parser_node_t *get_end_node(char **name, parser_t *tree) {
  if (tree->size == 0) {
    return NULL;
  }

  parser_node_t *end = NULL;
  for (size_t i = 0; i < tree->size; i++) {
    parser_node_t *tmp = &tree->node[i];
    size_t n = n_common_letters(*name, tmp);
    if (n == tmp->message.size) {
      *name += n;
      end = tmp;
      break;
    }
  }

  if (end == NULL) {
    return NULL;
  }

  parser_string_t string = {.data = (char *)*name, .size = strlen(*name)};

  bool keep_searching = true;
  while (keep_searching) {
    keep_searching = false;
    for (size_t n = 0; n < end->size; n++) {
      parser_node_t *selected_nd = &end->node[n];
      if (is_valid_node(&string, selected_nd)) {
        string.size -= selected_nd->message.size;
        string.data += selected_nd->message.size;

        end = selected_nd;
        keep_searching = true;
        break;
      }
    }
  }
  *name = string.data;
  return end;
}

void print_node(parser_node_t *nd) {
  size_t non_zero_sz = 0;
  for (size_t i = 0; i < nd->size; i++) {
    if (&nd->node[i] != NULL) {
      non_zero_sz++;
    }
  }
  printf("Nd: %s, value: %d, next sz: %zu, alloc sz: %zu\n", nd->message.data,
         nd->value, nd->size, non_zero_sz);
}

void insert_node(const char *full_name, int value, parser_node_t *nd){
  size_t n_same = n_common_letters(full_name, nd);
  const char *name = &full_name[n_same];
  size_t name_sz = strlen(name);

  int value2 = nd->value;
  parser_node_t *next2 = nd->node;
  size_t size2 = nd->size;

  // change current node
  nd->message.size = n_same;
  nd->value = (name_sz == 0) ? value : -1;
  nd->node = NULL;
  nd->size = 0;

  char *nd_name = &nd->message.data[n_same];
  add_node(nd_name, value2, nd);

  nd->node[0].node = next2;
  nd->node[0].size = size2;
}

void parser_add(const char *name, int const value, parser_t *tree) {
  char *part_name = (char *)name;
  parser_node_t *end_node = get_end_node(&part_name, tree);
  size_t part_name_size = strlen(part_name);
  if (part_name_size == 0){
    // replace node value because node already exists
    end_node->value = value;
    return;
  }

  if (end_node == NULL) {
#ifdef PARSER_DEBUG
    printf("+Adding: %s\n", name);
#endif
    add_root_node(name, value, tree);
    return;
  }

  // insert new intermediate node:
  // Hello, Hey --> He, llo, y
  // Hey, Hello --> He, y, llo
  // Hello, H --> H, ello
  // H, Hello --> H, ello

  bool is_first_letter_same = name[0] == end_node->message.data[0];
  bool is_name_shorter = strlen(name) <= end_node->message.size;
  if (is_first_letter_same && is_name_shorter) {
      insert_node(name, value, end_node);
  }

  bool is_part_name_empty = part_name[0] == 0;
  if (!is_part_name_empty) {
    add_node(part_name, value, end_node);
  }
}

int parser_parse(char const *name, parser_t *tree) {
  parser_string_t string = {.data = (char *)name, .size = strlen(name)};
  parser_node_t *nd = NULL;

  for (size_t n = 0; n < tree->size; n++) {
    parser_node_t *tmp = &tree->node[n];
    if (is_valid_node(&string, tmp)) {
      nd = tmp;
      break;
    }
  }
  if (nd == NULL) {
    return PARSER_MSG_NOT_FOUND;
  }

keep_searching:
  string.data += nd->message.size;
  string.size -= nd->message.size;
  if (string.size == 0){
    return nd->value;
  }

  for (size_t n = 0; n < nd->size; n++) {
    parser_node_t *tmp = &nd->node[n];
    if (is_valid_node(&string, tmp)){
      nd = tmp;
      goto keep_searching;
    }
  }

  return PARSER_MSG_NOT_FOUND;
}

#ifdef __cplusplus
}
#endif
