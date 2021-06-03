all: main.c libstack.a
	gcc -Wextra -c main.c
	gcc main.o -L. -lstack -o main


list: stack.c list.c
	gcc -Wextra -D USE_LIST -c stack.c list.c
	ar -rcs libstack.a stack.o list.o

vector: stack.c vector.c
	gcc -Wextra -c stack.c vector.c
	ar -rcs libstack.a stack.o vector.o