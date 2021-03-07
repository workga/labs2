#ifndef __VECTOR_H
#define __VECTOR_H


typedef struct Vector {
    void *data
    int size;
    int capacity;
} Vector;


Vector* vector_new();
void vector_delete(Vector *vector);
int vector_size(Vector *vector)

void vector_push_back(Vector *vector, void *data);
//void vector_push_front(Vector *vector, void *data);
//void vector_push(Vector *vector, Item *ptr, void *data);

void vector_remove_back(Vector *vector, Item *ptr);
//void vector_remove_front(Vector *vector, Item *ptr);
//void vector_remove(Vector *vector, Item *ptr);

int vector_empty(Vector *vector);
void vector_get(Vector *vector, int index, void *dest);


#endif