#include <stdlib.h>
#include <stdio.h>

#include "list.h"

void find_end(list_holder_t **list) {
  while ((*list)->next != NULL) {
    *list = (*list)->next;
  }
}

void list_append(void *data, list_holder_t *list) {
  find_end(&list);
  list_holder_t *new = (list_holder_t *)malloc(sizeof(list_holder_t));
  *new = (list_holder_t){.next = NULL, .prev = list, .data = data};
  list->next = new;
}

void list_free(list_holder_t *list) {
  find_end(&list);

  while (list->prev != NULL){
    list = list->prev;
    free(list->next);
  }
  free(list);
}

list_holder_t *list_init(){
  return (list_holder_t*)calloc(sizeof(list_holder_t), 1);
}

void *list_data(size_t depth, list_holder_t *list){
  while((depth+1) != 0){
    if (list->next != NULL){
      list = list->next;
      depth--;
    }
    else{
      return NULL;
    }
  }
  return list->data;
}