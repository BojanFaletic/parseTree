## Parser

Efficient string decoding algorithm. It is implemented as parse tree with minimal memory allocations.
This algorithm runs in linear time as function of input length. Naive searching would take N compares to all possible combinations,
`parser` dynamically prune imposable strings thus reduce search time.

## Warning
This is still in development and not yet sufficiently tested for deploying in production.

``` c
  parser_t *root;
  parser_init(&root);

  // set parsing values
  parser_add("Hello", 1, root);
  parser_add("Hey", 2, root);

  parser_parse("Hello", root);
  parser_parse("Hey", root);

  parser_free(root);
```

## Benchmark

Benchmark                   | Time            | CPU           |Iterations
 ---                        | ---             | ---           | ---
ParserParse                 |      10.3 ns    |     10.3 ns   |  64872132
NaiveParse                  |      12.3 ns    |     12.3 ns   |  56028453
Load_10_000_elements        | 223447185 ns    |223327596 ns   |         3
Parse_10_000_elements       |      33.8 ns    |     33.8 ns   |  20078692
Parse_10_000_elements_naive |     14606 ns    |    14600 ns   |     47617

Result of running Google Benchmark on `test/benchmark.cpp`. Parser is faster than naive strcmp.