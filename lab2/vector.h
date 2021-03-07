#ifndef __VECTOR_H
#define __VECTOR_H

#define VECTOR_CAPACITY 4

typedef struct Vector {
    void *data;
    size_t item_size;
    int size;
    int capacity;
} Vector;


Vector* vector_new(size_t item_size);
int vector_delete(Vector *vector);
int vector_size(Vector *vector);
int vector_capacity(Vector *vector);

int vector_push_back(Vector *vector, void *data);
//int vector_push_front(Vector *vector, void *data);
//int vector_push(Vector *vector, Item *ptr, void *data);

int vector_remove_back(Vector *vector);
//int vector_remove_front(Vector *vector);
//int vector_remove(Vector *vector, Item *ptr);

int vector_empty(Vector *vector);
int vector_get(Vector *vector, int index, void *dest);

int vector_print_int(Vector *vector);


#endif