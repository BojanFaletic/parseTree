#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "parser.h"

void check(const char *test, parser_t *root) {
  int status = parse(test, root);
  printf("Testing: %s: %d\n", test, status);
}

int test_parse() {
  parser_t *root = init_tree();

  node_t *tree = (node_t *)malloc(sizeof(node_t));
  tree->message = (string_t){.data = (char *)"He", .size = strlen("He")};
  tree->next = (basic_node_t){.node = 0, .size = 0};
  tree->value = 1;

  root->next = (basic_node_t){.node = tree, .size = 1};

  add_node("ab", 2, tree);
  add_node("cd", 3, tree);
  add_node("ef", 7, tree);

  check("He", root);
  check("Hecd", root);
  check("Heef", root);

  free_tree(root);
  return 0;
}


int test_add_word(){
  parser_t *root = init_tree();
  add_word("He", 1, root);

  free_tree(root);
  return 0;
}

int main() {

  //test_parse();
  test_add_word();
  return 0;
}