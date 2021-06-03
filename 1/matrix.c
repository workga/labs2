#include <stdlib.h>
#include <stdio.h>
#include "matrix.h"

int get_int(const char* format, int* address) {
	int error;
	do {
		error = scanf(format, address);
		if (error < 0) return 1;//EOF
		if (error == 0) {
			printf("Invalid input. Please, continue from the wrong place.\n");
			scanf("%*[^\n]");
		}
	} while (error == 0);
	return 0;
}

int get_matrix(Matrix* p_matrix) {
	printf("Please, enter the number of rows: ");
	do {
		if (get_int("%d", &(p_matrix->m))) {
			return 1;
		}

		if (p_matrix->m == 0) {
			p_matrix->rows = NULL;
			return 0;
		}

		if (p_matrix->m < 0) {
			printf("Invalid input. Please, continue from the wrong place.\n");
			scanf("%*[^\n]");
		}
	} while (p_matrix->m < 0);

	p_matrix->rows = (Row*)calloc(p_matrix->m, sizeof(Row));

	printf("Please, for each row enter its length and then enter its elements:\n");
	for(int i = 0; i < p_matrix->m; i++) {
		do {
			if (get_int("%d", &(p_matrix->rows[i].n))) {
				return 1;
			}

			if (p_matrix->rows[i].n < 0) {
				printf("Invalid input. Please, continue from the wrong place.\n");
				scanf("%*[^\n]");
			}
		} while (p_matrix->rows[i].n < 0);

		if (p_matrix->rows[i].n == 0) {
			p_matrix->rows[i].ints = NULL;
			continue;
		}

		p_matrix->rows[i].ints = (int*)calloc(p_matrix->rows[i].n, sizeof(int));

		for(int j = 0; j < p_matrix->rows[i].n; j++) {
			if (get_int("%d", &(p_matrix->rows[i].ints[j]))) return 1;
		}
	}

	return 0;
}

void delete_matrix(Matrix matrix) {
	for(int i = 0; i < matrix.m; i++) {
		free(matrix.rows[i].ints);
	}

	free(matrix.rows);
}

void print_matrix(char* message, Matrix matrix) {
	printf("%s\n", message);

	for(int i = 0; i < matrix.m; i++) {
		for(int j = 0; j < matrix.rows[i].n; j++) {
			printf("%5.d\t", matrix.rows[i].ints[j]);
		}
		printf("\n");
	}

	printf("\n");
}


int find_first_min(Row row) {
	if (row.n == 0) return -1;

	int start = 0;
	int min = row.ints[0];

	for(int j = 1; j < row.n; j++) {
		if(row.ints[j] < min) {
		    min = row.ints[j];
			start = j;
		}
	}

	return start;
}

Matrix process_matrix(Matrix old_matrix) {
	Matrix new_matrix;
	new_matrix.m = old_matrix.m;
	new_matrix.rows = (Row*)calloc(new_matrix.m, sizeof(Row));

	for(int i = 0; i < old_matrix.m; i++) {
		if (old_matrix.rows[i].n == 0) {
			new_matrix.rows[i].n = 0;
			new_matrix.rows[i].ints = NULL;
			continue;
		}

		int start = find_first_min(old_matrix.rows[i]);

		new_matrix.rows[i].n = old_matrix.rows[i].n - start;
		new_matrix.rows[i].ints = (int*)calloc(new_matrix.rows[i].n, sizeof(int));

		for(int j = 0; j < new_matrix.rows[i].n; j++) {
			new_matrix.rows[i].ints[j] = old_matrix.rows[i].ints[j + start];
		}
	}

	return new_matrix;
}
