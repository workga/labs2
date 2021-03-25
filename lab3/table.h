#ifndef __TABLE_H
#define __TABLE_H

typedef struct Space_1 Space_1;
typedef struct Space_2 Space_2;

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


typedef struct Table {
	Space_1 *space_1;
	Space_2 *space_2;
} Table;


Table *table_new(int space_1_max_size,
	             int space_2_max_size,
	             int space_2_max_key_len);

int table_insert(int key_1, int parent_key_1, char *key_2);

Data* table_find(int key_1, char *key_2);

void data_print(Data *data);

#endif