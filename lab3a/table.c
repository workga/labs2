#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "table.h"
#include "space_1.h"
#include "space_2.h"
#include "data.h"


Table *table_new(int space_1_max_size,
				 int space_2_max_size,
				 int space_2_max_key_len) {
	if (VERBOSE) printf("[INFO] table_new\n");

	Space_1 *space_1 = space_1_new(space_1_max_size);
	Space_2 *space_2 = space_2_new(space_2_max_size, space_2_max_key_len);

	Table *table = (Table*)calloc(1, sizeof(Table));

	if (!space_1 || !space_2 || !table) {
		space_1_delete(space_1);
		space_2_delete(space_2);
		free(table);
		return NULL;
	}

	table->space_1 = space_1;
	table->space_2 = space_2;

	return table;
}

void table_delete(Table *table) {
	if (VERBOSE) printf("[INFO] table_delete\n");

	table_remove_all(table);
	space_1_delete(table->space_1);
	space_2_delete(table->space_2);
	free(table);
}


int table_insert(Table *table, int key_1, int parent_key_1, char *key_2, Info info) {
	if (VERBOSE) printf("[INFO] table_insert\n");

	if (space_2_check_key(table->space_2, key_2)) return 1;
	if (table_find(table, key_1, key_2)) return 2;

	Data *data = data_new(info);
	if (!data) return 3;

	if (space_1_insert(table->space_1, key_1, parent_key_1, data)) {
		data_delete(data);
		return 4;
	}

	if (space_2_insert(table->space_2, key_2, data)) {
		space_1_remove(table->space_1, key_1, 0, NULL);
		data_delete(data);
		return 5;
	};

	return 0;
}


Data* table_find_data(Table *table, int key_1, char *key_2) {
	if (VERBOSE) printf("[INFO] table_find_data\n");

	Data *data =  space_1_find(table->space_1, key_1);

	if (!data || strcmp(data->key_2, key_2) != 0) return NULL;
	else return data;
}


const Info* table_find(Table *table, int key_1, char *key_2) {
	if (VERBOSE) printf("[INFO] table_find\n");

	Data* data = table_find_data(table, key_1, key_2);

	if (!data) return NULL;
	else return data->info;
}


int table_remove(Table *table, int key_1, char *key_2, int recurr) {
	if (VERBOSE) printf("[INFO] table_remove\n");

	if (space_2_check_key(table->space_2, key_2)) return 1;
	Data* data = table_find_data(table, key_1, key_2);
	if (!data) return 2;


	Data_array data_array;
	data_array.array = (Data**)calloc(table->space_1->max_size, sizeof(Data*));
	data_array.len = 0;
	if (!data_array.array) return 3;

	space_1_remove(table->space_1, key_1, recurr, &data_array);

	for (int i = 0; i < data_array.len; i++) {
		Data* data = data_array.array[i];
		space_2_remove(table->space_2, data->key_2, data->release);
		data_delete(data);
	}

	free(data_array.array);

	return 0;
}

int table_remove_all(Table *table) {
	if (VERBOSE) printf("[INFO] table_remove_all\n");

	Data_array data_array;
	data_array.array = (Data**)calloc(table->space_1->max_size, sizeof(Data*));
	data_array.len = 0;
	if (!data_array.array) return 3;

	space_1_remove_all(table->space_1, &data_array);

	for (int i = 0; i < data_array.len; i++) {
		Data* data = data_array.array[i];
		space_2_remove(table->space_2, data->key_2, data->release);
		data_delete(data);
	}

	free(data_array.array);

	return 0;
}


void table_print(Table *table, int opt) {
	if (opt == 1) space_1_print(table->space_1);
	else if (opt == 2) space_2_print(table->space_2);
}
