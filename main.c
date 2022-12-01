#include <stdio.h>
#include "list.h"
#include "parser.h"
#include "loader.h"

void example(const char *word, parser_t *obj) {
  printf("Parser value: %d == %s\n", parser_parse(word, obj), word);
}

void test_item(const char *name, parser_t *root) {
  static int cnt = 0;
  parser_add(name, ++cnt, root);
  example(name, root);
}

int main() {
  // init parser object
  parser_t *root;
  parser_init(&root);

#if false
  // set parsing values
  parser_add("Hello", 1, root);
  parser_add("Hey", 2, root);
  parser_add("World", 3, root);
  parser_add("Word2", 4, root);


  const char *data1 = "Hello";
  const char *data2 = "World";
  const char *data3 = "Hey";
  const char *data4 = "War";

  example(data1, root);
  example(data2, root);
  example(data3, root);
  example(data4, root);
#endif

  json_obj_t *list;
  parser_load_json("test_cases/add_new.json", root, &list);

  example("WM",  root);
  example("WMK",  root);
  example("WMU",  root);
  example("WMA",  root);

  // freeing object
  parser_free(root);
  parser_free_json(list);

  return 0;
}
