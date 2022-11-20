#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "parser.h"
#include "list.h"

void check(const char *test, parser_t *root) {
  int status = parse(test, root);
  printf("Testing: %s: %d\n", test, status);
}

int test_parse() {
  parser_t *root = init_tree();

  char* root_value = (char*)"He";
  add_word(root_value, 1, root);
  node_t *tree = get_end_node(&root_value, root);
  add_node("ab", 2, tree);
  add_node("cd", 3, tree);
  add_node("ef", 7, tree);
  add_node("gf", 7, tree);
  add_node("12", 7, tree);
  add_node("22", 7, tree);
  add_node("34", 7, tree);

  node_t *second = tree->node;
  add_node("se", 2, second);
  add_node("co", 2, second);

  //check("He", root);
  //check("Hecd", root);
  //check("Heef", root);

  free_tree(root);
  return 0;
}


int test_add_word(){
  parser_t *root = init_tree();
  add_word("H", 1, root);
  add_word("He", 2, root);
  add_word("Hel", 3, root);

  add_word("Ha", 4, root);

  check("Hel", root);
  check("Ha", root);
  check("He", root);

  free_tree(root);
  return 0;
}

int test_add_word2(){
  parser_t *root = init_tree();
  add_word("Hello", 1, root);
  add_word("Hey", 2, root);

  check("Hello", root);
  check("Hey", root);

  free_tree(root);
  return 0;
}

int test_list(){
  list_holder_t *list = list_init();
  list_append("Hello", list);
  list_append("World", list);

  printf("list: %s\n", (char*)list_data(0, list));
  printf("list: %s\n", (char*)list_data(1, list));

  printf("list depth: %zu\n", list_depth(list));

  list_free(list);
  return  0;
}

int main() {

  //test_parse();
  //test_add_word2();
  test_add_word2();
  //test_list();
  return 0;
}