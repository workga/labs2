#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>
#include <string.h>
#include "space_2.h"


Space_2* space_2_new(int max_size, int max_key_len) {
	if (VERBOSE) printf("[INFO] space_2_new\n");

	Space_2 *space = (Space_2*)calloc(1, sizeof(Space_2));
	Item_2 *items = (Item_2*)calloc(max_size, sizeof(Item_2));

	if (!space || !items) {
		free(items);
		free(space);
		return NULL;
	}

	for (int i = 0; i < max_size; i++) {
		items[i].head = NULL;
		items[i].release = 0;
	}

	space->max_size = max_size;
	space->max_key_len = max_key_len;
	space->items = items;

	return space;
}


void node_2_delete(Node_2 *node) {
	free(node->key);
	free(node);
}


void space_2_remove_list(Node_2 *head) {
	if (VERBOSE) printf("[INFO] space_2_remove_list\n");

	Node_2 *cur = head;
	Node_2 *prev = cur;
	while (cur)	{
		cur = cur->next;
		node_2_delete(prev);
		prev = cur;
	}
}


void space_2_delete(Space_2 *space) {
	if (VERBOSE) printf("[INFO] space_2_delete\n");

	if (!space) return;

	for (int i = 0; i < space->max_size; i++) {
		space_2_remove_list(space->items[i].head);
	}

	free(space->items);
	free(space);
}


int space_2_hash(Space_2 *space, char *key) {
	if (VERBOSE) printf("[INFO] space_2_hash\n");
	int h = 0;
	for (int i = 0; i < strlen(key); i++) {
		h = h^(int)key[i];
	}
	h %= space->max_size;
	return h;
}


int space_2_check_key(Space_2 *space, char *key) {
	if (VERBOSE) printf("[INFO] space_2_check_key\n");

	if (strlen(key) > space->max_key_len) return 1;
	else return 0;
}


int space_2_insert(Space_2 *space, char *key, Data* data) {
	if (VERBOSE) printf("[INFO] space_2_insert\n");

	if (space_2_check_key(space, key)) return 1;
	int index = space_2_hash(space, key);


	Node_2 *node = (Node_2*)calloc(1, sizeof(Node_2));
	char *key_copy = (char*) calloc(strlen(key) + 1, sizeof(key));
	if (!node || !key_copy) {
		free(key_copy);
		free(node);
		return 2;
	}
    strcpy(key_copy, key);

	space->items[index].release++;

	node->key = key_copy;
	node->release = space->items[index].release;
	node->data = data;
	node->next = space->items[index].head;
	space->items[index].head = node;

	return 0;
}


Data* space_2_find(Space_2 *space, char *key, int release) {
	if (VERBOSE) printf("[INFO] space_2_find\n");
	if (space_2_check_key(space, key)) return NULL;
	int index = space_2_hash(space, key);

	Node_2 *cur = space->items[index].head;
	while (cur) {
		if ((cur->release == release) && (strcmp(cur->key, key) == 0)) {
			return cur->data;
		}
		cur = cur->next;
	}

	return NULL;
}


Space_2* space_2_find_all(Space_2 *space, char *key) {
	if (VERBOSE) printf("[INFO] space_2_find_all\n");

	if (space_2_check_key(space, key)) return NULL;
	int index = space_2_hash(space, key);

	Space_2 *new_space = space_2_new(space->max_size, space->max_key_len);
	if (!new_space) return NULL;

	Node_2 *cur = space->items[index].head;
	while (cur) {
        if (strcmp(cur->key, key) == 0) {
            space_2_insert(new_space, cur->key, cur->data);
		}

		cur = cur->next;
	}

	return new_space;
}


int space_2_remove(Space_2 *space, char *key, int release) {
	if (VERBOSE) printf("[INFO] space_2_remove\n");

	if (space_2_check_key(space, key)) return 1;
	int index = space_2_hash(space, key);

	Node_2 *cur = space->items[index].head;
	Node_2 *prev = NULL;
	while (cur) {
		if ((cur->release == release) && (strcmp(cur->key, key) == 0)) {
			if (!prev) { //cur is head
				space->items[index].head = cur->next;
			} else {
				prev->next = cur->next;
			}

			node_2_delete(cur);

			return 0;
		}
		prev = cur;
		cur = cur->next;
	}

	return 1;
}

void space_2_print(Space_2 *space) {
	printf("\n");
    printf("---------/ SPACE_2 /------------\n");
    printf("hash\tlast\tlist\n");
	for (int i = 0; i < space->max_size; i++) {
		Item_2 item = space->items[i];

		printf("%d\t%d\t|", i, item.release);

		Node_2 *cur = item.head;
		while (cur) {
			printf("--> (key: %s, release: %d) ", cur->key, cur->release);
			cur = cur->next;
		}
		printf("\n");
	}
	printf("--------------------------------\n");
	printf("\n");
}
