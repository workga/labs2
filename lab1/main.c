//- add input errors
//- use matrix pointers
//- write tester
//- push it all

#include "matrix.h"

int main(int argc, const char** argv) {
	Matrix old_matrix = get_matrix();
	Matrix new_matrix = process_matrix(old_matrix);

	print_matrix("Original matrix:", old_matrix);
	print_matrix("Processed matrix:", new_matrix);

	delete_matrix(old_matrix);
	delete_matrix(new_matrix);

	return 0;
}
