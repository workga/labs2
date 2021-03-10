//lab 2.3

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "stack.h"

#define MAX_INT_LEN 4

int fill_stack(Stack *stack);
Stack* reverse_stack(Stack *old_stack);
double* step(Stack *stack);



int main(int argc, const char** argv) {
	Stack* stack = stack_new();
	if (fill_stack(stack)) {
		printf("[INFO] ERROR: STACK FILLED WITH NOT-NULL STATUS");
		stack_delete(stack);
		return 1;
	}

	printf("[INFO] STACK: ");
	stack_print(stack);

	stack = reverse_stack(stack);
	printf("[INFO] REVERSED STACK: ");
	stack_print(stack);

	double* res = step(stack);


	if (res) printf("[INFO] RESULT: %f\n", *res);
	free(res);
	stack_delete(stack);

	return 0;
}


int fill_stack(Stack *stack) {
	char ch;
	char buf[MAX_INT_LEN + 1];
	int len = 0;

	while((ch = getchar()) != EOF) {
		if isdigit(ch) {
			if (len >= MAX_INT_LEN) return 1;

			buf[len] = ch;
			len++;
		} else {
			if (len > 0) {
				buf[len] = '\0';
				if (stack_push(stack, buf, strlen(buf) + 1)) return 1; //VECTOR OVERFLOW
				len = 0;
			}

			if ((ch == '+') || (ch == '-') || (ch == '*') || (ch == '/')) {
				buf[0] = ch;
				buf[1] = '\0';
				if (stack_push(stack, buf, strlen(buf) + 1)) return 1; //VECTOR OVERFLOW
			} else if (ch == '\n') {
				break;
			} else if (ch != ' ') return 1;
		}
	}

	if (ch == EOF) return 1;

	return 0;
}


double* step(Stack *stack) {
	char *op = stack_pop(stack);
	printf("[INFO] POP OPERATOR: %s \n", op);

	if(!op) return NULL;

	double *res = (double*)malloc(sizeof(double));

	if isdigit(op[0]) {
		*res = (double)atoi(op);
		printf("[INFO] OPERATOR IS INTEGER: %f \n", *res);

	} else {

		double* op_right = step(stack); //right order, because the stack is reversed
		double* op_left = step(stack);

		if (!op_right || !op_left) {
			printf("[INFO] WRONG INPUT");
			free(res);
			res = NULL;
		}
		else {

			if (strcmp(op, "+") == 0) {
				printf("[INFO] SUM: \"%s\"\n", op);
				*res = (*op_left) + (*op_right);
			}
			else if (strcmp(op, "-") == 0) {
				printf("[INFO] DIF: \"%s\"\n", op);
				*res = (*op_left) - (*op_right);
			}
			else if (strcmp(op, "*") == 0) {
				printf("[INFO] MULT: \"%s\"\n", op);
				*res = (*op_left) * (*op_right);
			}
			else if (strcmp(op, "/") == 0) {
				printf("[INFO] DIV: \"%s\"\n", op);
				*res = (*op_left) / (*op_right);
			}

			printf("[INFO] INTERMEDIATE RESULT: %f \n", *res);
		}

		free(op_left);
		free(op_right);

	}

	free(op);
	return res;
}

Stack* reverse_stack(Stack *old_stack) {
	Stack *new_stack = stack_new();

	char *buf;
	while(buf = stack_pop(old_stack)) {
		stack_push(new_stack, buf, strlen(buf) + 1);
		free(buf);
	}

	stack_delete(old_stack);
	return new_stack;
}