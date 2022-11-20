#include "tests.h"
#include <stdio.h>


int main() {
  int status = test_parser();
  if (status != 0) {
    printf("Tests failed\n");
  } else {
    printf("Tests PARSER_SUCCESS\n");
  }
  return status;
}