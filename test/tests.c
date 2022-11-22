#include <stdio.h>
#include <stdlib.h>

#include "list.h"
#include "parser.h"
#include "tests.h"

int check(const char *test, int expects, parser_t *root) {
  int status = parser_parse(test, root);
  if (status != expects) {
    printf("Testing failed: %s: %d != %d\n", test, status, expects);
    return PARSER_MSG_NOT_FOUND;
  }
  return PARSER_SUCCESS;
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

int test_item(const char *name, parser_t *nd) {
  static int id = 0;
  parser_add(name, ++id, nd);
  return check(name, id, nd);
}

int test_add_random() {
  parser_t *root;
  parser_init(&root);

  int status = PARSER_SUCCESS;

  status |= test_item("H", root);
  status |= test_item("He", root);
  status |= test_item("Hell", root);
  status |= test_item("Hello", root);
  status |= test_item("Wee", root);
  status |= test_item("W", root);

  status |= test_item("He", root);
  status |= test_item("Hi", root);
  status |= test_item("Hj", root);
  status |= test_item("Hjcc", root);

  parser_free(root);
  return status;
}

int test_list() {
  list_holder_t *list;
  list_init(&list);

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
  status |= test_parser_add();
  status |= test_parser_add2();
  status |= test_parser_add3();
  status |= test_add_random();

  return status;
}
