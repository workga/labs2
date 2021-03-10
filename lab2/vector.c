#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include "vector.h"

Vector* vector_new() {
	Vector *vector = (Vector*)malloc(sizeof(Vector));
	vector->items = (Item*)calloc(VECTOR_CAPACITY, sizeof(Item));
	vector->size = 0;
	vector->capacity = VECTOR_CAPACITY;

	return vector;
}

int vector_delete(Vector *vector) {
	for (int i = 0; i < vector->size; i++) {
		free(vector->items[i].data);
	}
	free(vector->items);
	free(vector);

	return 0;
}

int vector_size(Vector *vector) {
	return vector->size;
}

int vector_capacity(Vector *vector) {
	return vector->capacity;
}



int vector_push_back(Vector *vector, void *data, size_t size) {
	if(vector->size == vector->capacity) return 1;

	Item *ptr = vector->items + vector->size;
	ptr->data = (void*)malloc(size);
	ptr->size = size;
	memcpy(ptr->data, data, size);
	vector->size++;

	return 0;
}

// int vector_push_front(Vector *vector, void *data, size_t size) {
// 
// }

// int vector_push(Vector *vector, Item *ptr, void *data, size_t size) {
// 
// }



int vector_remove_back(Vector *vector) {
	if (vector->size == 0) return 1;

	Item *ptr = vector->items + (vector->size - 1);
	free(ptr->data);
	vector->size--;

	return 0;
}

// int vector_remove_front(Vector *vector) {
// 
// }

// int vector_remove(Vector *vector, Item *ptr) {
// 
// }



int vector_empty(Vector *vector) {
	if (vector->size == 0) return 1;
	else return 0;
}

void *vector_get(Vector *vector, int index) {
	if ((index > vector->size - 1) || (index < 0) || (vector->size == 0)) return NULL;

	Item *ptr = vector->items + index;
    void *data = (void*)malloc(ptr->size);
	memcpy(data, ptr->data, ptr->size);

	return data;
}



int vector_print(Vector *vector) {
	printf("\"");
	for (int i = 0; i < vector->size; i++) {
		printf("%s ", (char*)vector->items[i].data);
	}
	printf("\"");
	printf("\n");
	return 0;
}