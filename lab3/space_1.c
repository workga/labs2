#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>
#include "space_1.h"


Space_1* space_1_new(int max_size) {
    if (VERBOSE) printf("[INFO] space_1_new\n");

	Space_1 *space = (Space_1*)calloc(1, sizeof(Space_1));
	Item_1 *items = (Item_1*)calloc(max_size, sizeof(Item_1));

	if (!space || !items) {
		free(space);
		free(items);
		return NULL;
	}


	space->max_size = max_size;
	space->size = 0;
	space->items = items;

	return space;
}


void space_1_delete(Space_1 *space) {
	if (VERBOSE) printf("[INFO] space_1_delete\n");

	if (!space) return;
	
	free(space->items);
	free(space);
}


int space_1_insert(Space_1 *space, unsigned int key, unsigned int parent_key, Data* data) {
	if (VERBOSE) printf("[INFO] space_1_insert\n");

	if (space_1_find(space, key)) return 1;
	if ((parent_key != 0) && !space_1_find(space, parent_key)) return 2;


	if (space->size >= space->max_size) {
		space_1_collect_garbage(space);
		if (space->size >= space->max_size) return 3;
	}

	space->items[space->size].busy = 1;
	space->items[space->size].key = key;
	space->items[space->size].parent_key = parent_key;
	space->items[space->size].data = data;
	space->size++;
	return 0;
}


int space_1_remove(Space_1 *space, unsigned int key, int recurr) {
	if (VERBOSE) printf("[INFO] space_1_remove\n");

	int index = space_1_find_index(space, key);
	if (index < 0) return 1;

	space->items[index].busy = 0;

	for(int i = 0; i < space->size; i++) {
		if (space->items[i].parent_key == key) {
			if (recurr) space_1_remove(space, space->items[i].key, 1);
			else space->items[i].parent_key = 0;
			// maybe dont do it? 
			// (maybe change it also in Data?)
		}
	}


	return 0;
}


int space_1_find_index(Space_1 *space, unsigned int key) {
    if (VERBOSE) printf("[INFO] space_1_find_index\n");

    Item_1 item;
	for(int i = 0; i < space->size; i++) {
		item = space->items[i];
		if (item.key == key) {
			if (item.busy) return i;
			else return -1;
		}
	}

	return -1;
}

Data* space_1_find(Space_1 *space, unsigned int key) {
    if (VERBOSE) printf("[INFO] space_1_find\n");

    Item_1 item;
	for(int i = 0; i < space->size; i++) {
		item = space->items[i];
		if (item.key == key) {
			if (item.busy) return item.data;
			else return NULL;
		}
	}

	return NULL;
}


void space_1_collect_garbage(Space_1* space) {
    if (VERBOSE) printf("[INFO] space_1_collect_garbage\n");

	int j = 0;

	for (int i = 0; i < space->size; i++) {
		if (space->items[i].busy == 1) {
			space->items[j] = space->items[i];
			j++;
		}
	}

	space->size = j;
}


void space_1_print(Space_1 *space) {
    printf("\n");
    printf("---------/ SPACE_1 /------------\n");
    printf("busy\tkey\tparent_key\n");
	for (int i = 0; i < space->size; i++) {
		printf("%d\t%d\t%d\n",
			space->items[i].busy,
			space->items[i].key,
			space->items[i].parent_key
		);
	}
	printf("--------------------------------\n");
	printf("\n");
}
