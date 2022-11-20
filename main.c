#include "tests.h"
#include <stdio.h>

int main() {
  int status = test_parser();
  if (status != 0) {
    printf("Tests failed\n");
  } else {
    printf("Tests success\n");
  }
  return status;
}