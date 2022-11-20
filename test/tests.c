#include <stdio.h>
#include <stdlib.h>

#include "list.h"
#include "parser.h"
#include "tests.h"

int check(const char *test, int expects, parser_t *root) {
  int status = parser_parse(test, root);
  if (status != expects) {
    printf("Testing failed: %s: %d\n", test, status);
    return PARSER_MSG_NOT_FOUND;
  }
  return PARSER_SUCCESS;
}

int test_parse() {
  parser_t *root;
  parser_init(&root);

  char *root_value = (char *)"He";
  parser_add(root_value, 1, root);
  parser_node_t *tree = get_end_node(&root_value, root);
  add_node("ab", 2, tree);
  add_node("cd", 3, tree);
  add_node("ef", 7, tree);
  add_node("gf", 7, tree);
  add_node("12", 7, tree);
  add_node("22", 7, tree);
  add_node("34", 7, tree);

  parser_node_t *second = tree->node;
  add_node("se", 2, second);
  add_node("co", 2, second);

  int status = PARSER_SUCCESS;
  status |= check("He", 1, root);
  status |= check("Hecd", 3, root);
  status |= check("Heef", 7, root);

  parser_free(root);
  return status;
}

int test_parser_add() {
  parser_t *root;
  parser_init(&root);

  parser_add("H", 1, root);
  parser_add("He", 2, root);
  parser_add("Hel", 3, root);

  parser_add("Ha", 4, root);

  int status = PARSER_SUCCESS;
  status |= check("Hel", 3, root);
  status |= check("Ha", 4, root);
  status |= check("He", 2, root);

  parser_free(root);
  return status;
}

int test_parser_add2() {
  parser_t *root;
  parser_init(&root);

  parser_add("Hello", 1, root);
  parser_add("Hey", 2, root);

  int status = PARSER_SUCCESS;
  status |= check("Hello", 1, root);
  status |= check("Hey", 2, root);

  parser_free(root);
  return status;
}

int test_parser_add3() {
  parser_t *root;
  parser_init(&root);

  parser_add("Hello", 1, root);
  parser_add("H", 2, root);

  int status = PARSER_SUCCESS;
  status |= check("Hello", 1, root);
  status |= check("H", 2, root);

  parser_free(root);
  return status;
}

int test_list() {
  list_holder_t *list = list_init();
  list_append("Hello", list);
  list_append("World", list);

  printf("list: %s\n", (char *)list_data(0, list));
  printf("list: %s\n", (char *)list_data(1, list));

  printf("list depth: %zu\n", list_depth(list));

  list_free(list);
  return 0;
}

int test_parser() {

  int status = PARSER_SUCCESS;
  status |= test_parse();
  status |= test_parser_add();
  status |= test_parser_add2();
  status |= test_parser_add3();

  return status;
}