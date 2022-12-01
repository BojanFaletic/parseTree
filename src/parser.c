#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "list.h"
#include "parser.h"

#define MIN(A, B) ((A < B) ? A : B)

///////////////////////////////////////////////////////////////////////////////
// Static function prototypes
///////////////////////////////////////////////////////////////////////////////

#if PARSER_DEBUG
// Debug
static void print_list(list_holder_t *list);
static void print_node(parser_node_t *nd);
#endif

// Utility
static void make_empty_node(const char *name, int const value,
                            parser_node_t *child);
static bool is_valid_node(parser_string_t *name, parser_node_t *node);
static void map_all_nodes(parser_node_t *nd, list_holder_t *list);
static void free_all_nodes_in_list(list_holder_t *list);
static void link_node(parser_node_t *parent, parser_node_t *child);
static void link_root_node(parser_t *parent, parser_node_t *child);
static void add_node(const char *name, int const value, parser_node_t *node);
static void add_root_node(const char *name, int const value, parser_t *node);
static size_t n_common_letters(const char *name, parser_node_t const *nd);
static int get_end_node(char **name, parser_t *tree, parser_node_t **end);
///////////////////////////////////////////////////////////////////////////////
// Parser Functions
///////////////////////////////////////////////////////////////////////////////

void parser_free(parser_t *tree) {
  list_holder_t *list;
  list_init(&list);

  // add first layer
  printf("Node size: %zu\n", tree->size);
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

void insert_node(const char *full_name, int value, parser_node_t *nd) {
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

  size_t id = nd->size - 1;
  nd->node[id].node = next2;
  nd->node[id].size = size2;
}

void parser_add(const char *name, int const value, parser_t *tree) {
  char *part_name = (char *)name;
  parser_node_t *end_node;
  int end_nd_status = get_end_node(&part_name, tree, &end_node);
  size_t part_name_size = strlen(part_name);

  switch (end_nd_status) {
  case 0:
    // insert node at the end
    break;
  case 1:
    /* code */
    break;
  case 2:
    // node already exists assign new value
    if (part_name_size == 0) {
#ifdef PARSER_DEBUG
      printf("Warning: %s already exists!, overriding value\n", name);
#endif
      end_node->value = value;
      return;
    }

    break;

  case 3:
    // insert root node
#ifdef PARSER_DEBUG
    printf("+Adding: %s\n", name);
#endif
    add_root_node(name, value, tree);
    break;

  case 4:
    // split old node, create new node, assign its value
    insert_node(name, value, end_node);
    break;

  default:
    break;
  }
}

int parser_parse(char const *name, parser_t *tree) {
  parser_string_t string = {.data = (char *)name, .size = strlen(name)};
  parser_node_t *nd = NULL;

  for (size_t n = 0; n < tree->size; n++) {
    parser_node_t *candidate = &tree->node[n];
    if (is_valid_node(&string, candidate)) {
      nd = candidate;
      break;
    }
  }
  if (nd == NULL) {
    return PARSER_MSG_NOT_FOUND;
  }

keep_searching:
  string.data += nd->message.size;
  string.size -= nd->message.size;
  if (string.size == 0) {
    return nd->value;
  }

  for (size_t n = 0; n < nd->size; n++) {
    parser_node_t *candidate = &nd->node[n];
    if (is_valid_node(&string, candidate)) {
      nd = candidate;
      goto keep_searching;
    }
  }

  return PARSER_MSG_NOT_FOUND;
}

///////////////////////////////////////////////////////////////////////////////
// Utility Functions
///////////////////////////////////////////////////////////////////////////////

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

static void map_all_nodes(parser_node_t *nd, list_holder_t *list) {
  if (nd->size != 0) {
    printf("Node size: %zu\n", nd->size);
    list_append(nd->node, list);
#ifdef PARSER_DEBUG
    printf("Adding: %zu\n", nd->size);
#endif
  }

  for (size_t i = 0; i < nd->size; i++) {
    parser_node_t *candidate = &nd->node[i];
    map_all_nodes(candidate, list);
  }
}

static void free_all_nodes_in_list(list_holder_t *list) {
  list_end(&list);
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

static void link_node(parser_node_t *parent, parser_node_t *child) {
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

static void make_empty_node(const char *name, int const value,
                            parser_node_t *child) {
  *child = (parser_node_t){
      .node = NULL,
      .size = 0,
      .message = (parser_string_t){.data = (char *)name, .size = strlen(name)},
      .value = value};
}

static void link_root_node(parser_t *parent, parser_node_t *child) {
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

static void add_node(const char *name, int const value, parser_node_t *node) {
  parser_node_t child;
  make_empty_node(name, value, &child);
  link_node(node, &child);
#ifdef PARSER_DEBUG
  printf("+Adding: %s\n", name);
#endif
}

static void add_root_node(const char *name, int const value, parser_t *node) {
  parser_node_t child;
  make_empty_node(name, value, &child);
  link_root_node(node, &child);
#ifdef PARSER_DEBUG
  printf("+Adding: %s\n", name);
#endif
}

static size_t n_common_letters(const char *name, parser_node_t const *nd) {
  size_t const max_search = MIN(strlen(name), nd->message.size);
  size_t i;

  for (i = 0; i < max_search; i++) {
    if (name[i] != nd->message.data[i]) {
      return i;
    }
  }
  return i;
}

static int get_end_node(char **name, parser_t *tree, parser_node_t **end) {
  size_t name_sz = strlen(*name);
  size_t tmp_sz = tree->size;
  parser_node_t *tmp_nd = tree->node;
  int return_code = 3;

  bool found_node = false;
keep_searching:
  for (size_t i = 0; i < tmp_sz; i++) {
    parser_node_t *candidate = &tmp_nd[i];
    size_t n = n_common_letters(*name, candidate);
    if (n == candidate->message.size && name_sz <= candidate->message.size) {
      // partial node already exists, but has no value
      *name += n;
      found_node = true;
      tmp_nd = candidate;

      return_code = 2;
      break;
    if (n != candidate->message.size && name_sz <= candidate->message.size){
      // partial node does not already exist need to insert node
      *name += n;
      found_node = true;
      tmp_nd = candidate;
      return_code = 4;
      break;
    }
    } else if (n == candidate->message.size) {
      // name_sz is still larger than node size, continue searching
      *name += n;
      name_sz -= n;

      tmp_nd = candidate;
      tmp_sz = candidate->size;
      found_node = true;
      return_code = 0;
      goto keep_searching;
    } else if (n != 0) {
      // found some partial matching with two nodes, but no exact solution
      *name += n;
      tmp_nd = candidate;
      found_node = true;
      return_code = 1;
      break;
    }
  }

  *end = (found_node) ? tmp_nd : NULL;
  return return_code;
}

///////////////////////////////////////////////////////////////////////////////
// Debug Functions
///////////////////////////////////////////////////////////////////////////////
#if PARSER_DEBUG
static void print_list(list_holder_t *list) {
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

static void print_node(parser_node_t *nd) {
  size_t non_zero_sz = 0;
  for (size_t i = 0; i < nd->size; i++) {
    if (&nd->node[i] != NULL) {
      non_zero_sz++;
    }
  }
  printf("Nd: %s, value: %d, next sz: %zu, alloc sz: %zu\n", nd->message.data,
         nd->value, nd->size, non_zero_sz);
}
#endif
