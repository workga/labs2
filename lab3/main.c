// - add error msgs to returns
// - add Data_array_new()
// - add table_find_1() and table_remove_1() (and 2 - it is space)

#include "data.h"
#include "space_1.h"
#include "space_2.h"
#include "table.h"
#include "menu.h"

int VERBOSE = 1;

const int SPACE_1_MAX_SIZE    = 10;
const int SPACE_2_MAX_SIZE    = 3;
const int SPACE_2_MAX_KEY_LEN = 3;

int main(int argc, const char** argv) {
	Table *table = table_new(SPACE_1_MAX_SIZE,
						 	 SPACE_2_MAX_SIZE,
						 	 SPACE_2_MAX_KEY_LEN);

	//EXAMPLE:
	Info info = {10, 20, "boom"};
	table_insert(table, 1, 0, "a", info);
	table_insert(table, 2, 1, "a", info);
	table_insert(table, 3, 1, "b", info);
	table_insert(table, 4, 0, "c", info);
	table_insert(table, 5, 4, "d", info);



	start(table);

	table_delete(table);
	return 0;
}

//---------------------------------------------------------------------------------

// void test_table() {
// 	Table *table = table_new(SPACE_1_MAX_SIZE,
// 							 SPACE_2_MAX_SIZE,
// 							 SPACE_2_MAX_KEY_LEN);

// 	table_print(table, 1);
// 	table_print(table, 2);

// 	Info info = {10, 20, "boom"};

// 	table_insert(table, 1, 0, "a", info);
// 	table_insert(table, 2, 1, "a", info);
// 	table_insert(table, 2, 2, "a", info);// wrong
// 	table_insert(table, 3, 4, "a", info);// wrong
// 	table_insert(table, 3, 1, "b", info);
// 	table_insert(table, 4, 0, "c", info);
// 	table_insert(table, 5, 4, "d", info);
// 	table_insert(table, 6, 2, "e", info);//wrong
// 	table_insert(table, 6, 2, "long", info);

// 	table_print(table, 1);
// 	table_print(table, 2);



// 	table_remove(table, 4, "c", 1);

// 	table_print(table, 1);
// 	table_print(table, 2);

// 	table_remove_all(table);

// 	table_print(table, 1);
// 	table_print(table, 2);



// 	table_insert(table, 1, 0, "a", info);
// 	table_insert(table, 2, 1, "a", info);
// 	table_insert(table, 2, 2, "a", info);// wrong
// 	table_insert(table, 3, 4, "a", info);// wrong

// 	table_print(table, 1);
// 	table_print(table, 2);

// 	table_delete(table);
// }


// void test_space(int num, int verbose) {
// 	VERBOSE = verbose;

//  Info *inf = (Info*)calloc(1, sizeof(Info));
// 	Data *data = (Data*)calloc(1, sizeof(Data));

// 	inf->int_1 = 10;
// 	inf->int_2 = 20;
// 	inf->str = "boom";

// 	data->info = inf;
// 	data->key_1 = 0;
// 	data->key_2 = "";


//     if (num == 1) {

// 		Space_1 *space_1 = space_1_new(SPACE_1_MAX_SIZE);
// 		space_1_print(space_1);

// 		space_1_insert(space_1, 1, 0, data);
// 		space_1_print(space_1);
// 		space_1_insert(space_1, 2, 1, data);
// 		space_1_print(space_1);
// 		space_1_insert(space_1, 2, 2, data);//wrong
// 		space_1_print(space_1);
// 		space_1_insert(space_1, 3, 4, data);//wrong
// 		space_1_print(space_1);
// 		space_1_insert(space_1, 3, 1, data);
// 		space_1_print(space_1);
// 		space_1_insert(space_1, 4, 0, data);
// 		space_1_print(space_1);
// 		space_1_insert(space_1, 5, 4, data);
// 		space_1_print(space_1);

// 		printf("Find index: %d\n", space_1_find_index(space_1, 2));
// 		printf("Find index: %d\n", space_1_find_index(space_1, 6));
// 		data_print(space_1_find(space_1, 2));

// 		space_1_remove(space_1, 4, 0);
// 		space_1_print(space_1);
// 		space_1_remove(space_1, 1, 1);
// 		space_1_print(space_1);

// 		space_1_collect_garbage(space_1);
// 		space_1_print(space_1);


// 		space_1_delete(space_1);

//     } else if (num == 2) {

// 		Space_2 *space_2 = space_2_new(SPACE_2_MAX_SIZE, SPACE_2_MAX_KEY_LEN);
// 		space_2_print(space_2);

// 		space_2_insert(space_2, "aa1", data);
// 		space_2_print(space_2);
// 		space_2_insert(space_2, "aa1", data);
// 		space_2_print(space_2);
// 		space_2_insert(space_2, "aa2", data);
// 		space_2_print(space_2);
// 		space_2_insert(space_2, "aa3", data);
// 		space_2_print(space_2);
// 		space_2_insert(space_2, "aa4", data);
// 		space_2_print(space_2);
// 		space_2_insert(space_2, "long", data);// wrong
// 		space_2_print(space_2);


// 		printf("Find:\n");
// 		data_print(space_2_find(space_2, "aa1", 1));
// 		printf("Find:\n");
// 		data_print(space_2_find(space_2, "aa1", 3)); //wrong
// 		printf("Find:\n");
// 		data_print(space_2_find(space_2, "long", 1)); //wrong

// 		printf("---------/ SPACE_2_RESULT /------\n");
// 		Space_2 *result = space_2_find_all(space_2, "aa1");
// 		space_2_print(result);
// 		space_2_delete(result);
// 		printf("---------------------------------\n");


// 		space_2_remove(space_2, "aa1", 1);
// 		space_2_print(space_2);
// 		space_2_remove(space_2, "aa4", 3);
// 		space_2_print(space_2);
// 		space_2_remove(space_2, "long", 1); // wrong
// 		space_2_print(space_2);


// 		space_2_delete(space_2);
// 	}

// 	free(inf);
// 	free(data);
// }
