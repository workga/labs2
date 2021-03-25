#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>

#include "table.h"
#include "space_1.h"
#include "space_2.h"


Table *table_new(int space_1_max_size,
				 int space_2_max_size,
				 int space_2_max_key_len) {

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


int table_insert(int key_1, int parent_key_1, char *key_2) {

}


Data* table_find(int key_1, char *key_2) {

}


void data_print(Data *data) {
	if (!data) printf("Data: NULL\n");
	else printf("Data: %d\n", data->info->int_1);
}