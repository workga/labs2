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


Stack stack_new(size_t item_size) {
	Container *cont = (Container*)malloc(sizeof(Container));

	#ifdef USE_LIST

	cont->list = list_new(item_size);

	#else

	cont->vector = vector_new(item_size);

	#endif


	Stack stack= {cont};
	return stack;
}

int stack_delete(Stack s) {
	#ifdef USE_LIST

	list_delete(s.cont->list);

	#else

	vector_delete(s.cont->vector);

	#endif

	free(s.cont);

	return 0;
}

int stack_push(Stack s, void* data) {
	#ifdef USE_LIST

	list_push_front(s.cont->list, data);

	#else

	vector_push_back(s.cont->vector, data);

	#endif

	return 0;
}

int stack_pop(Stack s, void* dest) {
	#ifdef USE_LIST

	list_get_front(s.cont->list, dest);
	list_remove_front(s.cont->list);

	#else

	int size = vector_size(s.cont->vector);
	vector_get(s.cont->vector, size - 1, dest);
	vector_remove_back(s.cont->vector);

	#endif

	return 0;
}

int stack_peek(Stack s, void* dest) {
	#ifdef USE_LIST

	list_get_front(s.cont->list, dest);

	#else

	int size = vector_size(s.cont->vector);
	vector_get(s.cont->vector, size - 1, dest);

	#endif

	return 0;
}

int stack_print_int(Stack s) {
	#ifdef USE_LIST

	list_print_int(s.cont->list);

	#else

	vector_print_int(s.cont->vector);

	#endif

	return 0;
}