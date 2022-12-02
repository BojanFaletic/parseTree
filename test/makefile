CC = clang
CBUILD = -Wall -Wextra --std=c17
SRCS = main.c tests.c ../src/parser.c ../src/list.c ../src/loader.c
INC = -I../inc

.PHONY: benchmark release tests syntax

all: $(SRCS)
	$(CC) $^ $(CBUILD) -o main -fsanitize=address $(INC)

syntax: $(SRCS)
	$(CC) $^ $(CBUILD) -Wpedantic -Werror -fsyntax-only -O0 $(INC)

static: $(SRCS)
	$(CC) $^ $(CBUILD) -o main -fsanitize=address -Wpedantic -Werror $(INC)

release: $(SRCS)
	$(CC) $^ $(CBUILD) -o main -O2 -Wpedantic -Werror $(INC)

tests: $(SRCS)
	$(CC) $(CBUILD) ../src/parser.c -I../inc -c -O2 --std=c17
	$(CC) $(CBUILD) ../src/list.c -I../inc -c -O2 --std=c17
	$(CC) $(CBUILD) ../src/loader.c -I../inc -c -O2 --std=c17
	$(CC)++ test.cpp loader.o parser.o list.o -I../inc -O2 --std=c++20 -lgtest -pthread -o test

PARSER_DEBUG: $(SRCS)
	$(CC) $^ $(CBUILD) -o main -fsanitize=address -DPARSER_DEBUG $(INC)

benchmark:
	$(CC) $(CBUILD) ../src/parser.c -I../inc -c -O2
	$(CC) $(CBUILD) ../src/list.c -I../inc -c -O2
	$(CC) $(CBUILD) ../src/loader.c -I../inc -c -O2 --std=c17
	$(CC)++ benchmark.cpp loader.o parser.o list.o -I../inc --std=c++20 -isystem benchmark/include -lbenchmark -lpthread -o benchmark

gen_tests: gen_tests.c
	$(CC) $^ $(CBUILD) -o $@ -fsanitize=address

clear:
	@rm -f main benchmark *.o
