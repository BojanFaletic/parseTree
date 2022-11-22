#include "parser.h"
#include <stdio.h>

void example(const char *word, parser_t *obj) {
  switch (parser_parse(word, obj)) {
  case 1:
    printf("Parser found: %d == %s\n", 1, word);
    break;

  case 2:
    printf("Parser found: %d == %s\n", 2, word);
    break;

  case 3:
    printf("Parser found: %d == %s\n", 3, word);
    break;

  default:
    printf("Error unknown input: %s\n", word);
    break;
  }
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

  test_item("Hello", root);
  test_item("Hell", root);

  // freeing object
  parser_free(root);
  return 0;
}
