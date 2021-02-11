#include <stdlib.h>
#include <stdio.h>
#include "matrix.h"

Matrix get_matrix() {
	Matrix matrix;
	printf("Please, enter the number of rows:\n");
	scanf("%d", &(matrix.m));
	matrix.rows = (Row*)calloc(matrix.m, sizeof(Row));

	printf("Please, for each row enter its length and then enter its elements:\n");
	for(int i = 0; i < matrix.m; i++) {
		scanf("%d", &(matrix.rows[i].n));
		matrix.rows[i].ints = (int*)calloc(matrix.rows[i].n, sizeof(int));

		for(int j = 0; j < matrix.rows[i].n; j++) {
			scanf("%d", &(matrix.rows[i].ints[j]));
		}
	}

	return matrix;
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
		int start = find_first_min(old_matrix.rows[i]);

		new_matrix.rows[i].n = old_matrix.rows[i].n - start;
		new_matrix.rows[i].ints = (int*)calloc(new_matrix.rows[i].n, sizeof(int));

		for(int j = 0; j < new_matrix.rows[i].n; j++) {
			new_matrix.rows[i].ints[j] = old_matrix.rows[i].ints[j + start];
		}
	}

	return new_matrix;
}
