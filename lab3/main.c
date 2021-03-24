#include <stdlib.h>
#include <stdio.h>
#include "space_1.h"

#define SPACE_1_MAX_SIZE 10
#define SPACE_2_MAX_SIZE 3
#define SPACE_2_MAX_KEY_LEN 10

const int VERBOSE = 0;

typedef struct Info {
	int int_1;
	int int_2;
	char *str;
} Info;

typedef struct Data {
	Info *info;
	unsigned int key_1;
	char *key_2;
} Data;

void print_data(Data *data);


int main(int argc, const char** argv) {
	Info *inf = (Info*)calloc(1, sizeof(Info));
	Data *data = (Data*)calloc(1, sizeof(Data));

	inf->int_1 = 10;
	inf->int_2 = 20;
	inf->str = "boom";

	data->info = inf;
	data->key_1 = 0;
	data->key_2 = "";


	//......./ test space_1 /...........
	Space_1 *space_1 = space_1_new(SPACE_1_MAX_SIZE);


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

	printf("Find key: %d\n", space_1_find(space_1, 2));
    printf("Find key: %d\n", space_1_find(space_1, 6));
    print_data(space_1_get(space_1, space_1_find(space_1, 2)));

    space_1_remove(space_1, 4, 0);
    space_1_print(space_1);
    space_1_remove(space_1, 1, 1);
    space_1_print(space_1);

    space_1_collect_garbage(space_1);
    space_1_print(space_1);


    space_1_delete(space_1);

	//..................................

	free(inf);
	free(data);
 	return 0;
}

void print_data(Data *data) {
    printf("Get data: %d\n", data->info->int_1);
}
