//lab 1.25
#include <stdio.h>
#include "matrix.h"

int main(int argc, const char** argv) {
	Matrix old_matrix;
	if (get_matrix(&old_matrix)) {
		printf("Interrupted by user.");
		return 1;
	}
	
	Matrix new_matrix = process_matrix(old_matrix);

	print_matrix("Original matrix:", old_matrix);
	print_matrix("Processed matrix:", new_matrix);

	delete_matrix(old_matrix);
	delete_matrix(new_matrix);

	return 0;
}
