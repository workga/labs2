#ifndef __LIST_H
#define __LIST_H


typedef struct Item {
    void *data;
    struct Item *next;
} Item;

typedef struct List {
    Item *head;
    Item *tail;
    size_t item_size;
} List;


List* list_new(size_t item_size);
int list_delete(List *list);
int item_delete(Item *item);
//int list_is_empty(List *list);

//int list_push_back(List *list, void *data);
int list_push_front(List *list, void *data);
//int list_push(List *list, Item *ptr, void *data);

//int list_remove_back(List *list);
int list_remove_front(List *list);
//int list_remove(List *list, Item *ptr);

//int list_get_back(List *list, void *dest);
int list_get_front(List *list, void *dest);
//int list_get(List list, Item *ptr, void *dest);

int list_print_int(List *list);


#endif