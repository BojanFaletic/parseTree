#include <assert.h>
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
static void make_empty_node(parser_string_t st, int const value,
                            parser_node_t *child);
static bool is_valid_node(parser_string_t *name, parser_node_t *node);
static void map_all_nodes(parser_node_t *nd, list_holder_t *list);
static void free_all_nodes_in_list(list_holder_t *list);
static void link_node(parser_node_t *parent, parser_node_t *child);
static void link_root_node(parser_t *parent, parser_node_t *child);
static void add_node(parser_string_t st, int const value, parser_node_t *node);
static void add_root_node(const char *name, int const value, parser_t *node);
static size_t n_common_letters(const char *name, parser_node_t const *nd);
static parser_node_t *get_end_node(char **name, parser_t *tree, int *action);
///////////////////////////////////////////////////////////////////////////////
// Parser Functions
///////////////////////////////////////////////////////////////////////////////

void parser_free(parser_t *tree) {
  list_holder_t *list;
  list_init(&list);

#ifdef PARSER_DEBUG
  printf("Node size: %zu\n", tree->size);
#endif
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

void insert_node_chain(const char *full_name, int value, parser_node_t *nd) {
  size_t n_same = n_common_letters(full_name, nd);
  const char *name = &full_name[n_same];
  size_t name_sz = strlen(name);

  int value2 = nd->value;
  parser_node_t *next2 = nd->node;
  size_t size2 = nd->size;

  // change current node
  nd->message.size = n_same;
  nd->value = (name_sz == 0) ? value : nd->value;
  nd->node = NULL;
  nd->size = 0;

  // add new node at end
  char *chain_name = &nd->message.data[n_same];
  parser_string_t pr = {.data = chain_name, .size = strlen(chain_name)};
  add_node(pr, value2, nd);
  nd->node[0].node = next2;
  nd->node[0].size = size2;
}

void insert_node_branch(const char *full_name, int value, parser_node_t *nd) {
  size_t n_same = n_common_letters(full_name, nd);

  int value2 = nd->value;
  parser_node_t *next2 = nd->node;
  size_t size2 = nd->size;

  // change current node
  parser_string_t orig_msg = {.data = nd->message.data + n_same,
                              .size = nd->message.size - n_same};

  nd->message.size = n_same;
  nd->value = -1;
  nd->node = NULL;
  nd->size = 0;

  // original branch
  add_node(orig_msg, value2, nd);
  assert(nd->size == 1);
  nd->node[0].node = next2;
  nd->node[0].size = size2;

  // new branch
  parser_string_t branch_msg = {.data = (char *)&full_name[n_same],
                                .size = strlen(&full_name[n_same])};
  add_node(branch_msg, value, nd);
}

void parser_add(const char *name, int const value, parser_t *tree) {
  char *part_name = (char *)name;
  int action;
  parser_node_t *end_node = get_end_node(&part_name, tree, &action);

  if (end_node == NULL) {
    // root merge
    add_root_node(name, value, tree);
  } else if (action == 4) {
    // branch merge
    insert_node_branch(part_name, value, end_node);
  } else if (action == 3) {
    // insert node in chain
    insert_node_chain(part_name, value, end_node);
  } else if (action == 1) {
    // update value of node
    end_node->value = value;
  } else if (action == 5) {
    // normal adding of node
    parser_string_t pr = {.data = part_name, .size = strlen(part_name)};
    add_node(pr, value, end_node);
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
#ifdef PARSER_DEBUG
    printf("Node size: %zu\n", nd->size);
    printf("Adding: %zu\n", nd->size);
#endif
    list_append(nd->node, list);
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

static void make_empty_node(parser_string_t st, int const value,
                            parser_node_t *child) {
  *child =
      (parser_node_t){.node = NULL, .size = 0, .message = st, .value = value};
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

static void add_node(parser_string_t st, int const value, parser_node_t *node) {
  parser_node_t child;
  make_empty_node(st, value, &child);
  link_node(node, &child);
#ifdef PARSER_DEBUG
  printf("+Adding: %s\n", st.data);
#endif
}

static void add_root_node(const char *name, int const value, parser_t *node) {
  parser_node_t child;
  parser_string_t pr = {.data=(char*)name, .size=strlen(name)};
  make_empty_node(pr, value, &child);
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

int get_node_type(size_t n, size_t name_sz, size_t node_sz) {
  if (name_sz == 0) {
    return 1; // update just value
  }

  size_t max_search = MIN(name_sz, node_sz);
  if (n == max_search) {
    if (name_sz >= node_sz) {
      return 2; // keep searching
    }
    if (name_sz < node_sz) {
      return 3; // insert node in chain
    }
  }
  if (n != 0) {
    return 4; // insert node and branch
  }
  return 0; // do nothing
}

static parser_node_t *get_end_node(char **name, parser_t *tree, int *action) {
  size_t name_sz = strlen(*name);
  size_t tmp_sz = tree->size;
  parser_node_t *end_nd = (tmp_sz != 0) ? tree->node : NULL;
  parser_node_t *prev_nd = NULL;

  *action = 5;
keep_searching:
  for (size_t i = 0; i < tmp_sz; i++) {
    parser_node_t *candidate = &end_nd[i];
    size_t node_sz = candidate->message.size;
    size_t n = n_common_letters(*name, candidate);

    int status = get_node_type(n, name_sz, node_sz);

    if (status != 0) {
      if (status == 3) {
        *action = 3;
        prev_nd = candidate;
        break;
      }

      if (status == 4) {
        *action = 4;
        prev_nd = candidate;
        break;
      }

      if (status == 2) {
        *name += node_sz;
        name_sz -= node_sz;

        // if name == 0, then replace value of existing node
        if (name_sz == 0) {
          *action = 1;
          prev_nd = candidate;
          break;
        }

        if (candidate->node == NULL) {
          // return this node because next is null
          *action = 5;
          prev_nd = candidate;
          break;
        }

        prev_nd = candidate;
        tmp_sz = candidate->size;
        end_nd = candidate->node;
        goto keep_searching;
      } else {
        *action = status;
        break;
      }
    }
  }
  end_nd = prev_nd;
  return end_nd;
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
