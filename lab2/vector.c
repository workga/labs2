#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include "vector.h"

Vector* vector_new(size_t item_size) {
	Vector *vector = (Vector*)malloc(sizeof(Vector));
	vector->data = (void*)calloc(VECTOR_CAPACITY, item_size);
	vector->item_size = item_size;
	vector->size = 0;
	vector->capacity = VECTOR_CAPACITY;

	return vector;
}

int vector_delete(Vector *vector) {
	free(vector->data);
	free(vector);
}

int vector_size(Vector *vector) {
	return vector->size;
}

int vector_capacity(Vector *vector) {
	return vector->capacity;
}



int vector_push_back(Vector *vector, void *data) {
	if(vector->size == vector->capacity) return 1;

	void *ptr = vector->data + (vector->size)*(vector->item_size);
	memcpy(ptr, data, vector->item_size);
	vector->size++;

	return 0;
}

// int vector_push_front(Vector *vector, void *data) {
// 
// }

// int vector_push(Vector *vector, Item *ptr, void *data) {
// 
// }



int vector_remove_back(Vector *vector) {
	if (vector->size == 0) return 1;

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

int vector_get(Vector *vector, int index, void *dest) {
	if (index > vector->size - 1) return 1;

	void *ptr = vector->data + (index)*(vector->item_size);
	memcpy(dest, ptr, vector->item_size);

	return 0;
}



int vector_print_int(Vector *vector) {
	for (int index = 0; index < vector->size; index++) {
		void *ptr = vector->data + index*(vector->item_size);
		printf("%d ", *(int*)ptr);
	}
	printf("\n");
	return 0;
}