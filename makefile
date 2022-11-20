CBUILD = -Wall -Wextra --std=c17 -O0

all: main.c src/parser.c
	clang $^ $(CBUILD) -o main -fsanitize=address -DDEBUG -Iinc

main: main.c src/parser.c
	clang $^ $(CBUILD) -o main -fsanitize=address -Iinc

clear:
	@rm -f main
