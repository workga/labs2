#ifndef MATRIX_H
#define MATRIX_H

typedef struct Row {
	int n;
	int* ints;
} Row;

typedef struct Matrix {
	int m;
	Row* rows;
} Matrix;

int get_matrix(Matrix* p_matrix);
void delete_matrix(Matrix matrix);
void print_matrix(char* message, Matrix matrix);
int find_first_min(Row row);
Matrix process_matrix(Matrix old_matrix);

#endif
