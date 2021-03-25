#ifndef __SPACE_2_H
#define __SPACE_2_H

int VERBOSE;

typedef struct Data Data;

typedef struct Node_2 {
	char *key; // string with fixed length
	int release;
	Data *data;
	struct Node_2 *next;
} Node_2;

typedef struct Item_2 {
	int release;
	Node_2 *head;
} Item_2;

typedef struct Space_2 {
	int max_size;
	int max_key_len;
	Item_2 *items;
} Space_2;


Space_2* space_2_new(int max_size, int max_key_len);
void node_2_delete(Node_2 *node);
void space_2_remove_list(Node_2 *head);
void space_2_delete(Space_2 *space);

int space_2_check_key(Space_2 *space, char *key);
int space_2_hash(Space_2 *space, char *key);


int space_2_insert(Space_2 *space, char *key, Data* data);
Data* space_2_find(Space_2 *space, char *key, int release);
Space_2* space_2_find_all(Space_2 *space, char *key);
int space_2_remove(Space_2 *space, char *key, int release);
//change it: if release = 0 remove all and find first;

void space_2_print(Space_2 *space);

#endif
