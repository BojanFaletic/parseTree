all: main.cpp
	clang++ main.cpp -Wall -Wextra --std=c++20 -g -o main

.phony:
	rm -f main
