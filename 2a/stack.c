#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>
#include "stack.h"


#ifdef USE_LIST

#include "list.h"
//printf("[INFO] Using list.\n");

#else

#include "vector.h"
//printf("[INFO] Using vector.\n");

#endif


struct Container {
	#ifdef USE_LIST
	List *list;
	#else
	Vector *vector;
	#endif
};


Stack* stack_new() {
	Stack *s = (Stack*)malloc(sizeof(Stack));
	Container *cont = (Container*)malloc(sizeof(Container));

	#ifdef USE_LIST

	cont->list = list_new();

	#else

	cont->vector = vector_new();

	#endif


	s->cont = cont;
	return s;
}

int stack_delete(Stack *s) {
	#ifdef USE_LIST

	list_delete(s->cont->list);

	#else

	vector_delete(s->cont->vector);

	#endif

	free(s->cont);
	free(s);

	return 0;
}

int stack_push(Stack *s, void* data, size_t size) {
	#ifdef USE_LIST

	int status = list_push_front(s->cont->list, data, size);

	#else

	int status = vector_push_back(s->cont->vector, data, size);

	#endif

	return status;
}

void* stack_pop(Stack *s) {
	#ifdef USE_LIST

	void *data = list_get_front(s->cont->list);
	list_remove_front(s->cont->list);

	#else

	int size = vector_size(s->cont->vector);
	void *data = vector_get(s->cont->vector, size - 1);
	vector_remove_back(s->cont->vector);

	#endif

	return data;
}

void* stack_peek(Stack *s) {
	#ifdef USE_LIST

	void *data = list_get_front(s->cont->list);

	#else

	int size = vector_size(s->cont->vector);
	void *data = vector_get(s->cont->vector, size - 1);

	#endif

	return data;
}

int stack_print(Stack *s) {
	#ifdef USE_LIST

	list_print(s->cont->list);

	#else

	vector_print(s->cont->vector);

	#endif

	return 0;
}