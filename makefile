CBUILD = -Wall -Wextra --std=c17 -O0
SRCS = main.c src/parser.c src/list.c

all: $(SRCS)
	clang $^ $(CBUILD) -o main -fsanitize=address -DDEBUG -Iinc

main: $(SRCS)
	clang $^ $(CBUILD) -o main -fsanitize=address -Iinc

clear:
	@rm -f main
