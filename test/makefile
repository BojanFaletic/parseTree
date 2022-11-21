CBUILD = -Wall -Wextra --std=c17
SRCS = main.c tests.c ../src/parser.c ../src/list.c
INC = -I../inc

all: $(SRCS)
	clang $^ $(CBUILD) -o main -fsanitize=address $(INC)

static: $(SRCS)
	clang $^ $(CBUILD) -o main -fsanitize=address -Wpedantic -Werror $(INC)

PARSER_DEBUG: $(SRCS)
	clang $^ $(CBUILD) -o main -fsanitize=address -DPARSER_DEBUG $(INC)

clear:
	@rm -f main
