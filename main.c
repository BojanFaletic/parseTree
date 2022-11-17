#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

void clean_tree_recursive(node_t *node) {
  if (node->size == 0) {
    printf("Free node: %s \n", node->message.data);
    free(node);
    return;
  }
  for (size_t i = 0; i < node->size; i++) {
    clean_tree_recursive(node->next[i]);
  }
}

void clean_tree(root_node_t *tree) {
  for (size_t n = 0; n < tree->size; n++) {
    clean_tree_recursive(tree->nodes[n]);
  }
  printf("Free node pointers\n");
  free(tree);
}

root_node_t *init_tree() {
  root_node_t *tree = (root_node_t*)malloc(sizeof(root_node_t));
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

bool string_compare(string_t const *name, string_t const *node_string) {
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

int parse_recursive(string_t *string, node_t const *node) {
  if (string_compare(string, &node->message)){
    if (string->size == node->message.size){
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
  string_t string = {.data = (char*)name, .size=strlen(name)};
  for (size_t n = 0; n < tree->size; n++) {
    int status = parse_recursive(&string, tree->nodes[n]);
    if (status != MSG_NOT_FOUND) {
      return status;
    }
  }
  return MSG_NOT_FOUND;
}

int test_parse() {
  root_node_t *root = init_tree();

  node_t *tree = (node_t*)malloc(sizeof(node_t));
  string_t message = {.data=(char*)"He", .size=strlen("He")};
  tree->message = message;
  tree->value = 1;
  tree->size = 0;
  tree->next = 0;

  root->nodes = &tree;
  root->size = 1;

  add_node("ll", 2, tree);
  add_node("llo", 3, tree);
  int status;

  status = parse("He", root);
  printf("Status He: %d\n", status == 1);

  status = parse("Hello", root);
  printf("Status Hello: %d\n", status == 3);

  status = parse("Hell", root);
  printf("Status Hell: %d\n", status == 2);

  status = parse("Hellok", root);
  printf("Status Hellok: %d\n", status);

  clean_tree(root);
  return status;
}

int main() {

  test_parse();
  return 0;
}