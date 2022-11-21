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
ParserParse    |   16.4 ns     |    16.4 ns |    42451456
NaiveParse     |   14.9 ns     |    14.9 ns |    54427306

Currently still slower then simple strcmp. Parser need to have better optimizations for desired performance.