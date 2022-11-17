#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "parser.h"

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

  free_tree(root);
  return status;
}

int main() {

  test_parse();
  return 0;
}