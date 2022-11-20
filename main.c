#include "parser.h"
#include <stdio.h>

void example(const char *word, parser_t *obj) {
  switch (parse(word, obj)) {
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

int main() {
  // init parser object
  parser_t *root;
  init_tree(&root);

  // set parsing values
  add_word("Hello", 1, root);
  add_word("Hey", 2, root);

  const char *data1 = "Hello";
  const char *data2 = "World";

  example(data1, root);
  example(data2, root);

  // freeing object
  free_tree(root);
  return 0;
}