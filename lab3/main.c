// - add deep remove and delete with data_delete() and node_2_delete()
// - add error msgs to returns

#include <stdlib.h>
#include <stdio.h>
#include "table.h"
#include "space_1.h"
#include "space_2.h"

int VERBOSE = 1;

const int SPACE_1_MAX_SIZE    = 10;
const int SPACE_2_MAX_SIZE    = 3;
const int SPACE_2_MAX_KEY_LEN = 3;

void test_space(int num, int verbose);

//--------------------------------------
//-----------/ MAIN /-------------------
//--------------------------------------
int main(int argc, const char** argv) {
	test_space(1, 0);
	test_space(2, 0);


	return 0;
}
//--------------------------------------
//--------------------------------------
//--------------------------------------

void test_space(int num, int verbose) {
	VERBOSE = verbose;

    Info *inf = (Info*)calloc(1, sizeof(Info));
	Data *data = (Data*)calloc(1, sizeof(Data));

	inf->int_1 = 10;
	inf->int_2 = 20;
	inf->str = "boom";

	data->info = inf;
	data->key_1 = 0;
	data->key_2 = "";


    if (num == 1) {

		Space_1 *space_1 = space_1_new(SPACE_1_MAX_SIZE);
		space_1_print(space_1);

		space_1_insert(space_1, 1, 0, data);
		space_1_print(space_1);
		space_1_insert(space_1, 2, 1, data);
		space_1_print(space_1);
		space_1_insert(space_1, 2, 2, data);//wrong
		space_1_print(space_1);
		space_1_insert(space_1, 3, 4, data);//wrong
		space_1_print(space_1);
		space_1_insert(space_1, 3, 1, data);
		space_1_print(space_1);
		space_1_insert(space_1, 4, 0, data);
		space_1_print(space_1);
		space_1_insert(space_1, 5, 4, data);
		space_1_print(space_1);

		printf("Find index: %d\n", space_1_find_index(space_1, 2));
		printf("Find index: %d\n", space_1_find_index(space_1, 6));
		data_print(space_1_find(space_1, 2));

		space_1_remove(space_1, 4, 0);
		space_1_print(space_1);
		space_1_remove(space_1, 1, 1);
		space_1_print(space_1);

		space_1_collect_garbage(space_1);
		space_1_print(space_1);


		space_1_delete(space_1);

    } else if (num == 2) {

		Space_2 *space_2 = space_2_new(SPACE_2_MAX_SIZE, SPACE_2_MAX_KEY_LEN);
		space_2_print(space_2);

		space_2_insert(space_2, "aa1", data);
		space_2_print(space_2);
		space_2_insert(space_2, "aa1", data);
		space_2_print(space_2);
		space_2_insert(space_2, "aa2", data);
		space_2_print(space_2);
		space_2_insert(space_2, "aa3", data);
		space_2_print(space_2);
		space_2_insert(space_2, "aa4", data);
		space_2_print(space_2);
		space_2_insert(space_2, "long", data);// wrong
		space_2_print(space_2);


		printf("Find:\n");
		data_print(space_2_find(space_2, "aa1", 1));
		printf("Find:\n");
		data_print(space_2_find(space_2, "aa1", 3)); //wrong
		printf("Find:\n");
		data_print(space_2_find(space_2, "long", 1)); //wrong

		printf("---------/ SPACE_2_RESULT /------\n");
		Space_2 *result = space_2_find_all(space_2, "aa1");
		space_2_print(result);
		space_2_delete(result);
		printf("---------------------------------\n");


		space_2_remove(space_2, "aa1", 1);
		space_2_print(space_2);
		space_2_remove(space_2, "aa4", 3);
		space_2_print(space_2);
		space_2_remove(space_2, "long", 1); // wrong
		space_2_print(space_2);


		space_2_delete(space_2);
	}

	free(inf);
	free(data);
}
