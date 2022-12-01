#include "tests.h"
#include "parser.h"
#include "loader.h"
#include <stdio.h>
#include <time.h>


int main() {
  #if 0
  int status = test_parser();
  if (status != 0) {
    printf("Tests failed\n");
  } else {
    printf("Tests PARSER_SUCCESS\n");
  }
  return status;
  #else

  clock_t start, end;
  double cpu_time_used;

  parser_t *parse;
  json_obj_t *jobj;

  parser_init(&parse);
  parser_load_json("out.json", parse, &jobj);



  start = clock();
  parser_free_json(jobj);

  end = clock();
  parser_free(parse);

  cpu_time_used = ((double) (end - start)) / (CLOCKS_PER_SEC / 1000000);
  printf("Time used: %f us\n", cpu_time_used);


  return 0;

  #endif
}
