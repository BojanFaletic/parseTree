#pragma once
#include <stdlib.h>

// structure for holding list
typedef struct list_holder_t {
  struct list_holder_t *next, *prev;
  void *data;
} list_holder_t;

list_holder_t *list_init();
void list_append(void *data, list_holder_t *list);
void *list_data(size_t depth, list_holder_t *list);

void list_free(list_holder_t *list);