#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MSG_NOT_FOUND -1
#define SUCCESS 0

typedef struct parse_tree_t {
  struct parse_tree_t **next_nodes;
  size_t N_next;
  char *msg;
  size_t msg_len;
  int value;
} parse_tree_t;

void clean_tree(parse_tree_t *tree){
  if (tree->N_next == 0){
    printf("Free node: %s \n", tree->msg);
    free(tree);
    return;
  }
  for (size_t i=0; i<tree->N_next; i++){
    clean_tree(tree->next_nodes[i]);
  }

  printf("Free node pointers\n");
  free(tree->next_nodes);
}

void init_tree(const char *name, int const value, parse_tree_t *tree) {
  tree->next_nodes = 0;
  tree->N_next = 0;
  tree->msg = (char *)name;
  tree->msg_len = strlen(name);
  tree->value = value;
}

void add_node(const char *name, int const value,
              parse_tree_t *tree) {
  parse_tree_t **current_nodes = tree->next_nodes;
  size_t N = tree->N_next;

  parse_tree_t *new_node = (parse_tree_t *)malloc(sizeof(parse_tree_t));
  new_node->next_nodes = 0;
  new_node->N_next = 0;
  new_node->msg = (char *)name;
  new_node->msg_len = strlen(name);
  new_node->value = value;

  parse_tree_t **new_nodes =
      (parse_tree_t **)malloc(sizeof(parse_tree_t*) * (N + 1));


  for (size_t i = 0; i < N; i++) {
    new_nodes[i] = current_nodes[i];
  }
  new_nodes[N] = new_node;

  tree->next_nodes = new_nodes;
  tree->N_next = N + 1;

  free(current_nodes);
}

int parse(char const *name, size_t size, parse_tree_t *tree) {
  size_t node_size = tree->msg_len;

  if (size == node_size) {
    // check if value of node is correct
    for (size_t i = 0; i < node_size; i++) {
      if (name[i] != tree->msg[i]) {
        return -1;
      }
    }
    return tree->value;
  }

  return -1;
}

int test_parse() {
  parse_tree_t tree;
  init_tree("He", 1, &tree);
  add_node("ll", 2, &tree);
  add_node("lo", 3, &tree);

  int status = parse("He", 2, &tree);
  printf("Status: %d\n", status);


  clean_tree(&tree);
  return status;
}

int main() {

  test_parse();
  return 0;
}