all: main.c
	clang main.c -Wall -Wextra --std=c17 -o main -fsanitize=address

.phony:
	rm -f main
