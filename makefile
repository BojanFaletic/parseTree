CBUILD = -Wall -Wextra --std=c17 -O0

all: main.c parser.c
	clang $^ $(CBUILD) -o main -fsanitize=address -DDEBUG

.phony:
	rm -f main
