CBUILD = -Wall -Wextra --std=c17
SRCS = main.c tests.c ../src/parser.c ../src/list.c
INC = -I../inc

all: $(SRCS)
	clang $^ $(CBUILD) -o main -fsanitize=address $(INC)

static: $(SRCS)
	clang $^ $(CBUILD) -o main -fsanitize=address -Wpedantic -Werror $(INC)

PARSER_DEBUG: $(SRCS)
	clang $^ $(CBUILD) -o main -fsanitize=address -DPARSER_DEBUG $(INC)

benchmark:
	clang $(CBUILD) ../src/parser.c -I../inc -c
	clang $(CBUILD) ../src/list.c -I../inc -c
	clang++ benchmark.cpp parser.o list.o -I../inc --std=c++20 -isystem benchmark/include -lbenchmark -lpthread -o benchmark

clear:
	@rm -f main benchmark *.o
