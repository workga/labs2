#ifndef __SPACE_1_H
#define __SPACE_1_H

int VERBOSE;

typedef struct Data Data;

typedef struct Item_1 {
	char busy; // 0 or 1
	unsigned int key; // != 0 and unique
	unsigned int parent_key; // 0 means no parent
	Data *data;

} Item_1;

typedef struct Space_1 {
	int max_size;
	int size;
	Item_1 *items;
} Space_1;

Space_1* space_1_new(int max_size);
void space_1_delete(Space_1 *space);

int space_1_find_index(Space_1 *space, unsigned int key);
Data* space_1_find(Space_1 *space, unsigned int key);

void space_1_collect_garbage(Space_1 *space);

int space_1_insert(Space_1 *space, unsigned int key, unsigned int parent_key, Data* data);
int space_1_remove(Space_1 *space, unsigned int key, int recurr);

void space_1_print(Space_1 *space);

#endif
