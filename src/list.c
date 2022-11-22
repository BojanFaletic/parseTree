#include "list.h"

///////////////////////////////////////////////////////////////////////////////
// List Functions
///////////////////////////////////////////////////////////////////////////////

void list_init(list_holder_t **list) {
  *list = (list_holder_t *)calloc(sizeof(list_holder_t), 1);
}

void list_free(list_holder_t *list) {
  list_end(&list);
  while (list->prev != NULL) {
    list = list->prev;
    free(list->next);
  }
  free(list);
}

void list_append(void *data, list_holder_t *list) {
  if (list->data == NULL){
    list->data = data;
  }
  else{
    list_end(&list);
    list_holder_t *new = (list_holder_t *)malloc(sizeof(list_holder_t));
    *new = (list_holder_t){.next = NULL, .prev = list, .data = data};
    list->next = new;
  }
}

void *list_data(size_t depth, list_holder_t *list) {
  while (depth != 0) {
    if (list->next != NULL) {
      list = list->next;
      depth--;
    } else {
      return NULL;
    }
  }
  return list->data;
}

size_t list_depth(list_holder_t *list){
  size_t depth = 0;
  while (list->next != NULL){
    list = list->next;
    depth++;
  }
  return depth + 1;
}

void *list_end_item(list_holder_t *list){
  list_end(&list);
  return list->data;
}

void list_end(list_holder_t **list) {
  while ((*list)->next != NULL) {
    *list = (*list)->next;
  }
}
