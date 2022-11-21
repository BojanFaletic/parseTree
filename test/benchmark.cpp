#include <benchmark/benchmark.h>
#include "parser.h"

static void SingleParse(benchmark::State& state) {
  // Perform setup here
  // init parser object
  parser_t *root;
  parser_init(&root);

  // set parsing values
  parser_add("Hello", 1, root);
  parser_add("Hey", 2, root);

  for (auto _ : state) {
    // This code gets timed
    volatile int code = parser_parse("Hello", root);
  }
}
// Register the function as a benchmark
BENCHMARK(SingleParse);
// Run the benchmark
BENCHMARK_MAIN();