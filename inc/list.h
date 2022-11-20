#pragma once
#include <stdlib.h>

// structure for holding list
typedef struct list_holder_t {
  struct list_holder_t *next, *prev;
  void *data;
} list_holder_t;

// initiate list
list_holder_t *list_init();
// free list
void list_free(list_holder_t *list);

// add new item to end of list
void list_append(void *data, list_holder_t *list);

// get max depth of list
size_t list_depth(list_holder_t *list);

// get n item from list
void *list_data(size_t depth, list_holder_t *list);

// get end item from list
void *list_end(list_holder_t *list);

// change pointer of list to end
void find_end(list_holder_t **list);