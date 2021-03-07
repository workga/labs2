#ifndef STACK_H
#define STACK_H

typedef struct Container Container;

typedef struct Stack
{
	Container *cont;
} Stack;

Stack stack_new(size_t item_size);
int stack_delete(Stack s);
int stack_push(Stack s, void* data);
int stack_pop(Stack s, void* dest);
int stack_peek(Stack s, void* dest);
int stack_print_int(Stack s);

#endif