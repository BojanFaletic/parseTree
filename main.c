#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "parser.h"

int test_parse() {
  parser_t *root = init_tree();

  node_t *tree = (node_t *)malloc(sizeof(node_t));
  tree->message = (string_t) {.data = (char *)"He", .size = strlen("He")};
  tree->next = (basic_node_t) {.node = 0, .size = 0};
  tree->value = 1;

  root->next = (basic_node_t) {.node = tree, .size=1};

  add_node("ab", 2, tree);
  add_node("cd", 3, tree);
  add_node("ef", 7, tree);

  int status;

  //status = parse("He", root);
  //printf("Status He: %d\n", status == 1);

  status = parse("Heab", root);
  printf("Status Heab: %d\n", status);


  status = parse("Hecd", root);
  printf("Status Hecd: %d\n", status);

  status = parse("Hellok", root);
  printf("Status Hellok: %d\n", status);

  free_tree(root);
  return status;
}

int main() {

  test_parse();
  return 0;
}