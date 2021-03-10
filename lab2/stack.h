#ifndef STACK_H
#define STACK_H

typedef struct Container Container;

typedef struct Stack
{
	Container *cont;
} Stack;

Stack* stack_new();
int stack_delete(Stack *s);
int stack_push(Stack *s, void* data, size_t size);
void* stack_pop(Stack *s);
void* stack_peek(Stack *s);
int stack_print(Stack *s);

#endif