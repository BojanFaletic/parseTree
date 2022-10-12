all: main.cpp
	clang++ main.cpp -Wall -Wextra --std=c++20 -o main -fsanitize=address

.phony:
	rm -f main
