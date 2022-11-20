CBUILD = -Wall -Wextra --std=c17 -O0
SRCS = main.c src/parser.c src/list.c
INC = -Iinc


all: $(SRCS)
	clang $^ $(CBUILD) -o main -fsanitize=address $(INC)

release: $(SRCS)
	clang $^ $(CBUILD) -O2 -o main $(INC)

debug: $(SRCS)
	clang $^ $(CBUILD) -o main -fsanitize=address -DDEBUG $(INC)

clear:
	@rm -f main
