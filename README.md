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

Benchmark      | Time          | CPU        |Iterations
 ---           | ---           | ---        | ---
ParserParse    |   9.84 ns     |    9.84 ns |    69718658
NaiveParse     |   12.2 ns     |    12.2 ns |    56507811

Result of running Google Benchmark on `test/benchmark.cpp`. Parser is faster than naive strcmp.