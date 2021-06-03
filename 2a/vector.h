#ifndef __VECTOR_H
#define __VECTOR_H

#define VECTOR_CAPACITY 6

typedef struct Item {
    void *data;
    size_t size;
} Item;

typedef struct Vector {
    Item *items;
    int size;
    int capacity;
} Vector;


Vector* vector_new();
int vector_delete(Vector *vector);
int vector_size(Vector *vector);
int vector_capacity(Vector *vector);

int vector_push_back(Vector *vector, void *data, size_t size);
//int vector_push_front(Vector *vector, void *data, size_t size);
//int vector_push(Vector *vector, Item *ptr, void *data, size_t size);

int vector_remove_back(Vector *vector);
//int vector_remove_front(Vector *vector);
//int vector_remove(Vector *vector, Item *ptr);

int vector_empty(Vector *vector);
void* vector_get(Vector *vector, int index);

int vector_print(Vector *vector);


#endif