#ifndef __LIST_H
#define __LIST_H


typedef struct Item {
    void *data;
    size_t size;
    struct Item *next;
} Item;

typedef struct List {
    Item *head;
    Item *tail;
} List;


List* list_new();
int list_delete(List *list);
int item_delete(Item *item);
//int list_is_empty(List *list);

//int list_push_back(List *list, void *data);
int list_push_front(List *list, void *data, size_t size);
//int list_push(List *list, Item *ptr, void *data);

//int list_remove_back(List *list);
int list_remove_front(List *list);
//int list_remove(List *list, Item *ptr);

//void* list_get_back(List *list);
void* list_get_front(List *list);
//void* list_get(List list, Item *ptr);

int list_print(List *list);


#endif