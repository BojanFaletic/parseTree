#include <stdio.h>

typedef struct{
} parse_tree_t;

int parse(const char* name, size_t size, parse_tree_t const *tree){

    return -1;
}

int test_parse(){
    parse_tree_t tree;

    char str1[] = "He";

    int status = parse(str1, sizeof(str1), &tree);
    printf("Status: %u\n", status);

    return status;
}

int main(){

    test_parse();
    return 0;
}