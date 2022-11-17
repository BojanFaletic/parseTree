CBUILD = -Wall -Wextra --std=c17 -O0

all: main.c parser.c
	clang $^ $(CBUILD) -o main -fsanitize=address -DDEBUG

main: main.c parser.c
	clang $^ $(CBUILD) -o main -fsanitize=address

.phony:
	rm -f main
