#include <benchmark/benchmark.h>
#include "parser.h"
#include <string.h>

static void SingleParse(benchmark::State& state) {
  // Perform setup here
  // init parser object
  parser_t *root;
  parser_init(&root);

  // set parsing values
  parser_add("Hello", 1, root);
  parser_add("Hey", 2, root);
  parser_add("Heyll", 3, root);
  parser_add("Heyllaa", 4, root);

  const char *str = "Hekkk";


  for (auto _ : state) {
    // This code gets timed
    volatile int code = parser_parse(str, root);
  }
}

int test_naive(const char *st1){
  if (strcmp(st1, "Hello") == 0){
    return 1;
  }

  if (strcmp(st1, "Hey") == 0){
    return 2;
  }

  if (strcmp(st1, "Heyll") == 0){
    return 3;
  }

  if (strcmp(st1, "Heyllaa") == 0){
    return 4;
  }

  return -1;
}


static void NaiveParse(benchmark::State& state) {
  // Perform setup here
  // init parser object
  const char *str = "Hekkk";

  for (auto _ : state) {
    // This code gets timed
    test_naive(str);
  }
}
// Register the function as a benchmark
BENCHMARK(SingleParse);
BENCHMARK(NaiveParse);

// Run the benchmark
BENCHMARK_MAIN();