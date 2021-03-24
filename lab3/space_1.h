#ifndef __SPACE_1_H
#define __SPACE_1_H

const int VERBOSE;

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
// doesn't free Data!

int space_1_find(Space_1 *space, unsigned int key);
// returns index or -1 if doesn't exist

Data* space_1_get(Space_1 *space, int index);


void space_1_collect_garbage(Space_1 *space);

int space_1_insert(Space_1 *space, unsigned int key, unsigned int parent_key, Data* data);

int space_1_remove(Space_1 *space, unsigned int key, int recurr);
// doesn't free Data!
// recurr = 1 if recurrently else 0

void space_1_print(Space_1 *space);

#endif
