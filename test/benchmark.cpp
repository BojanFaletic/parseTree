#include "loader.h"
#include "parser.h"
#include <benchmark/benchmark.h>
#include <string.h>

static void ParserParse(benchmark::State &state) {
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

  parser_free(root);
}

int test_naive(const char *st1) {
  if (strcmp(st1, "Hello") == 0) {
    return 1;
  }

  if (strcmp(st1, "Hey") == 0) {
    return 2;
  }

  if (strcmp(st1, "Heyll") == 0) {
    return 3;
  }

  if (strcmp(st1, "Heyllaa") == 0) {
    return 4;
  }

  return -1;
}

static void NaiveParse(benchmark::State &state) {
  // Perform setup here
  // init parser object
  const char *str = "Hekkk";

  for (auto _ : state) {
    // This code gets timed
    test_naive(str);
  }
}

static void Load_10_000_elements(benchmark::State &state) {
  // Perform setup here
  // init parser object
  const char *str = "Hekkk";

  for (auto _ : state) {
    // This code gets timed
    parser_t *list;
    json_obj_t *json;

    parser_init(&list);
    parser_load_json("test_out.json", list, &json);

    parser_free_json(json);
    parser_free(list);
  }
}

static void Parse_10_000_elements(benchmark::State &state) {
  // Perform setup here
  // init parser object
  const char *str = "Hekkk";
  // This code gets timed
  parser_t *list;
  json_obj_t *json;

  parser_init(&list);
  parser_load_json("test_out.json", list, &json);

  for (auto _ : state) {
    parser_parse("SKFOJSC", list);
  }

  parser_free_json(json);
  parser_free(list);
}

static void Parse_10_000_elements_naive(benchmark::State &state) {
  // Perform setup here
  // init parser object
  const char *str = "Hekkk";

  for (auto _ : state) {
    // This code gets timed
    for (size_t i = 0; i < 10000; i++) {
      strcmp(str, "ASDFGHJJT");
    }
  }
}

// Register the function as a benchmark
BENCHMARK(ParserParse);
BENCHMARK(NaiveParse);
BENCHMARK(Load_10_000_elements);
BENCHMARK(Parse_10_000_elements);
BENCHMARK(Parse_10_000_elements_naive);

// Run the benchmark
BENCHMARK_MAIN();