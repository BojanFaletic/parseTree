#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef struct {
  size_t min_name, max_name;
  size_t num_entries;
  size_t seed;
  char *file_name;
} cfg_t;

typedef struct {
  char *name;
  int value;
} json_dict_t;

static cfg_t cfg;

void write_json(json_dict_t *j, const char *f_name) {
  FILE *fp = fopen(f_name, "w");
  if (fp == NULL) {
    fprintf(stderr, "Unable to open file: %s, err: %d\n", f_name, errno);
    return;
  }

  // write header
  fprintf(fp, "{\n");

  // write data
  const char *payload_normal = "   \"%s\" : %d,\n";
  const char *payload_special = "   \"%s\" : %d\n";

  for (size_t i = 0; i < cfg.num_entries - 1; i++) {
    fprintf(fp, payload_normal, j[i].name, j[i].value);
  }
  fprintf(fp, payload_special, j[cfg.num_entries - 1].name,
          j[cfg.num_entries - 1].value);

  // write footer
  fprintf(fp, "}\n");

  fclose(fp);
}

void make_entry(json_dict_t *j) {
  size_t min_val = 'A';
  size_t max_val = 'Z';

  size_t diff = max_val - min_val;
  size_t cfg_diff = cfg.max_name - cfg.min_name;

  size_t letter_sz = rand() % cfg_diff + cfg.min_name;
  j->name = malloc(sizeof(char) * letter_sz + 1);
  for (size_t i = 0; i < letter_sz; i++) {
    char letter = rand() % diff + min_val;
    j->name[i] = letter;
  }
  j->name[letter_sz] = 0;
  j->value = rand() % __INT16_MAX__;
}

void free_entry(json_dict_t *j) { free(j->name); }

void print_help() {
  printf("-min   -max  : size of word \n");
  printf("-N           : number of json lines \n");
  printf("-s           : random seed \n");
  printf("-f           : output file (.json)\n");
}

int check_input(int argc, char **argv) {

  // default init

  cfg_t tmp = {.min_name = 2,
               .max_name = 5,
               .num_entries = 100,
               .seed = 0,
               .file_name = NULL};

  cfg = tmp;

  for (int i = 2; i < argc; i++) {
    if (*argv[i] != '-') {
      if (strcmp("-min", argv[i - 1]) == 0) {
        cfg.min_name = atoi(argv[i]);
      } else if (strcmp("-max", argv[i - 1]) == 0) {
        cfg.max_name = atoi(argv[i]);
      } else if (strcmp("-N", argv[i - 1]) == 0) {
        cfg.num_entries = atoi(argv[i]);
      } else if (strcmp("-s", argv[i - 1]) == 0) {
        cfg.seed = atoi(argv[i]);
      } else if (strcmp("-f", argv[i - 1]) == 0) {
        cfg.file_name = argv[i];
      }
    }
  }

  if (cfg.file_name == 0) {
    return 1;
  }

  if (cfg.max_name <= cfg.min_name) {
    return 1;
  }

  // check input
  return 0;
}

int main(int argc, char **argv) {

  if (check_input(argc, argv)) {
    print_help();
    exit(1);
  }

  if (cfg.seed == 0) {
    srand(time(0));
  } else {
    srand(cfg.seed);
  }

  json_dict_t *j = malloc(sizeof(json_dict_t) * cfg.num_entries);

  for (size_t i = 0; i < cfg.num_entries; i++) {
    make_entry(&j[i]);
  }

  write_json(j, cfg.file_name);

  for (size_t i = 0; i < cfg.num_entries; i++) {
    free_entry(&j[i]);
  }
  free(j);

  return 0;
}