CBUILD = -Wall -Wextra --std=c17 -O0
SRCS = main.c test/tests.c src/parser.c src/list.c
INC = -Iinc -Itest

all: $(SRCS)
	clang $^ $(CBUILD) -o main -fsanitize=address -DDEBUG $(INC)

main: $(SRCS)
	clang $^ $(CBUILD) -o main -fsanitize=address -$(INC)

clear:
	@rm -f main
