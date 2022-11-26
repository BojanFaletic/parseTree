#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <errno.h>

#define MIN_NAME_LEN 5
#define MAX_NAME_LEN 10

#define JSON_ENTRIES 100000

typedef struct{
    char* name;
    int value;
}json_dict_t;

void write_json(json_dict_t *j, const char* f_name){
    FILE *fp = fopen(f_name, "w");
    if (fp == NULL){
        fprintf(stderr, "Unable to open file: %s, err: %d\n", f_name, errno);
        return;
    }

    // write header
    fprintf(fp, "{\n");

    // write data
    const char *payload_normal = "   \"%s\" : %d,\n";
    const char *payload_special = "   \"%s\" : %d\n";

    for (size_t i=0; i<JSON_ENTRIES-1; i++){
        fprintf(fp, payload_normal, j[i].name, j[i].value);
    }
    fprintf(fp, payload_special, j[JSON_ENTRIES-1].name, j[JSON_ENTRIES-1].value);

    // write footer
    fprintf(fp, "}\n");

    fclose(fp);
}

void make_entry(json_dict_t *j){
    size_t min_val = 'A';
    size_t max_val = 'Z';
    size_t diff = max_val - min_val;

    size_t letter_sz = rand() % (MAX_NAME_LEN - MIN_NAME_LEN) + MIN_NAME_LEN;
    j->name = malloc(sizeof(char) * letter_sz + 1);
    for (size_t i=0; i<letter_sz; i++){
        char letter = rand() % diff + min_val;
        j->name[i] = letter;
    }
    j->name[letter_sz] = 0;
    j->value = rand() % __INT16_MAX__;
}

void free_entry(json_dict_t *j){
    free(j->name);
}

int main(){
    size_t seed = 42; // time(0)
    srand(seed);

    json_dict_t *j = malloc(sizeof(json_dict_t) * JSON_ENTRIES);

    for (size_t i=0; i<JSON_ENTRIES; i++){
        make_entry(&j[i]);
    }

    write_json(j, "test_out.json");


    for (size_t i=0; i<JSON_ENTRIES; i++){
        free_entry(&j[i]);
    }
    free(j);

    return 0;
}