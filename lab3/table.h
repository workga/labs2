#ifndef __TABLE_H
#define __TABLE_H

typedef struct Space_1 Space_1;
typedef struct Space_2 Space_2;

typedef struct Info Info;
typedef struct Data Data;

typedef struct Table {
	Space_1 *space_1;
	Space_2 *space_2;
} Table;


Table *table_new(int space_1_max_size,
	             int space_2_max_size,
	             int space_2_max_key_len);

Table *table_delete(Table *table);

int table_insert(Table *table, int key_1, int parent_key_1, char *key_2, Info info);

Data* table_find_data(Table *table, int key_1, char *key_2);

const Info* table_find(Table *table, int key_1, char *key_2);

int table_remove(Table *table, int key_1, char *key_2, int recurr);
int table_remove_all(Table *table);

void table_print(Table *table, int opt);


#endif