#include "loader.h"
#include "list.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_STRING_SIZE 10

void load_json(const char *f_name, parser_t *obj, list_holder_t **json) {
  FILE *ptr = fopen(f_name, "r");
  if (ptr == NULL) {
    printf("File cannot be opened\n");
    return;
  }

  int json_context = 0;

  size_t key_counter = 0;
  size_t value_counter = 0;

  char key[MAX_STRING_SIZE] = {0};
  char value[MAX_STRING_SIZE] = {0};

  list_init(json);

  while (1) {
    char ch = fgetc(ptr);

    switch (json_context) {
    case 0:
      if (ch == '{') {
        json_context = 1;
      }
      break;

    case 1:
      if (ch == '\"') {
        json_context = 2;
      }
      break;

    case 2:
      // store name here
      if (ch == '\"') {
        json_context = 3;
      } else {
        key[key_counter++] = ch;
      }
      break;

    case 3:
      if (ch == ':') {
        json_context = 4;
      }
      break;

    case 4:
      // store value here
      if (ch != ',' && ch != '}') {
        value[value_counter++] = ch;
      }

      if (ch == ',' || ch == '}') {
        char *value_data = (char *)malloc(key_counter + 1);
        for (size_t i = 0; i < key_counter; i++) {
          value_data[i] = key[i];
        }
        value_data[key_counter] = 0;
        list_append(value_data, *json);

        // todo check if key is decoded correctly
        int int_value = atoi(value);
        parser_add(value_data, int_value, obj);

        // printf("%s : %s\n", key, value);
        for (size_t i = 0; i < key_counter; i++) {
          key[i] = 0;
        }
        key_counter = 0;
        value_counter = 0;
      }

      if (ch == ',') {
        json_context = 1;
      } else if (ch == '}') {
        json_context = 5;
      }
      break;

    default:
      break;
    }

    if (ch != EOF) {
      // printf("%c", ch);
    } else {
      break;
    }
  }
  fclose(ptr);
}

void json_free(list_holder_t *json) {
  list_end(&json);
  while (json->prev != NULL) {
    char *data = json->data;
    free(data);
    json = json->prev;
  }

  char *data = json->data;
  free(data);
  list_free(json);
}
