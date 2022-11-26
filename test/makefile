CC = clang
CBUILD = -Wall -Wextra --std=c17
SRCS = main.c tests.c ../src/parser.c ../src/list.c
INC = -I../inc

.PHONY: benchmark release

all: $(SRCS)
	$(CC) $^ $(CBUILD) -o main -fsanitize=address $(INC)

static: $(SRCS)
	$(CC) $^ $(CBUILD) -o main -fsanitize=address -Wpedantic -Werror $(INC)

release: $(SRCS)
	$(CC) $^ $(CBUILD) -o main -O2 -Wpedantic -Werror $(INC)


PARSER_DEBUG: $(SRCS)
	$(CC) $^ $(CBUILD) -o main -fsanitize=address -DPARSER_DEBUG $(INC)

benchmark:
	$(CC) $(CBUILD) ../src/parser.c -I../inc -c -O2
	$(CC) $(CBUILD) ../src/list.c -I../inc -c -O2
	$(CC)++ benchmark.cpp parser.o list.o -I../inc --std=c++20 -isystem benchmark/include -lbenchmark -lpthread -o benchmark

gen_tests: gen_tests.c
	$(CC) $^ $(CBUILD) -o $@ -fsanitize=address

clear:
	@rm -f main benchmark *.o
