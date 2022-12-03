CC:=clang
CBUILD:=-Wall -Wextra --std=c17
SRCS=main.c $(wildcard src/*c)
INC:=-Iinc

.PHONY:clear
all: basic

# Normal build with no optimization and address sanitizer
basic: $(SRCS)
	$(CC) $^ $(CBUILD) -o main -fsanitize=address $(INC)

# Build with optimization used for release
release: $(SRCS)
	$(CC) $^ $(CBUILD) -O2 -o main $(INC)

# Check for syntax errors
static: $(SRCS)
	$(CC) $^ $(CBUILD) -fsyntax-only -Wpedantic -Werror $(INC)

# Basic build with debug output
debug: $(SRCS)
	$(CC) $^ $(CBUILD) -o main -fsanitize=address -DPARSER_DEBUG $(INC)

clear:
	@rm -f main
