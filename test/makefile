CBUILD = -Wall -Wextra --std=c17
SRCS = main.c tests.c ../src/parser.c ../src/list.c
INC = -I../inc

all: $(SRCS)
	clang $^ $(CBUILD) -o main -fsanitize=address $(INC)

debug: $(SRCS)
	clang $^ $(CBUILD) -o main -fsanitize=address -DDEBUG $(INC)

clear:
	@rm -f main
