#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include "list.h"

List* list_new(size_t item_size) {
    List* list = (List *)calloc(1, sizeof(List));
    list->head = NULL;
    list->tail = NULL;
    list->item_size = item_size;
    return list;
}

int list_delete(List *list) {
    Item *ptr = list->head;
    Item *ptr_prev;
    while (ptr) {
        ptr_prev = ptr;
        ptr = ptr->next;
        item_delete(ptr_prev);
    }
    free(list);
    return 0;
}

int item_delete(Item *item) {
    free(item->data);
    free(item);
    return 0;
}

int list_is_empty(List *list) {
    if (list->head == NULL) {
        return 1;
    } else {
        return 0;
    }
}



// int list_push_back(List *list, void *data) {
// 
// }

int list_push_front(List *list, void *data) {
    Item *ptr = (Item*)malloc(sizeof(Item));
    ptr->data = (void*)malloc(list->item_size);
    memcpy(ptr->data, data, list->item_size);

    if (!list->head) {
        ptr->next = NULL;
        list->head = ptr;
        list->tail = ptr;
    } else {
        ptr->next = list->head;
        list->head = ptr;
    }

    return 0;
}

// int list_push(List *list, Item *ptr, void *data) {
// 
// }



// int list_remove_back(List *list) {
// 
// }

int list_remove_front(List *list) {
    Item *old_head = list->head;

    if (list->head == NULL) {
        return 1;
    } else if (list->head == list->tail) {
        list->head = NULL;
        list->tail = NULL;
    } else {
        list->head = list->head->next;
    }

    item_delete(old_head);

    return 0;
}

// int list_remove(List *list, Item *ptr) {
// 
//}



// int list_get_back(List *list, void *dest) {
// 
// }

int list_get_front(List *list, void *dest) {
    memcpy(dest, list->head->data, list->item_size);

    return 0;
}

// int list_get(List *list, Item *ptr, void* dest) {
// 
// }



int list_print_int(List *list) {
    for (Item *ptr = list->head; ptr; ptr = ptr->next) {
        printf("%d ", *(int*)(ptr->data));
    }
    printf("\n");

    return 0;
}