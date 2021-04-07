// [FEATURE] use only one traversal function
// [FEATURE] add timer
// [FEATURE] add loading from file
// [FEATURE] add beautiful drawing?

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "tree.h"




Node* node_new(Node *parent, Node *left, Node *right, char *key, char *info) {
	if (!key) return NULL;
	if (!info) return NULL;

	Node *node = (Node*)calloc(1, sizeof(Node));
	if (!node) return NULL;

	node->key = (char*)calloc((int)strlen(key) + 1, sizeof(char));
	if (!node->key) {
		free(node);
		return NULL;
	}

	node->info = (char*)calloc((int)strlen(info) + 1, sizeof(char));
	if (!node->info) {
		free(node->key);
		free(node);
		return NULL;
	}

	//-------------------------------------------------------------------------

	node->parent = parent;
	node->left   = left;
	node->right  = right;
	strcpy(node->key, key);
	strcpy(node->info, info);

	return node;
}


void node_delete(Node *node) {
	free(node->key);
	free(node->info);
	free(node);
}


Tree* tree_new() {
	Tree *tree = (Tree*)calloc(1, sizeof(Tree));
	if (!tree) return NULL;

	tree->root = NULL;
	tree->h    = 0;
	tree->n    = 0;

	return tree;
}


void tree_delete(Tree *tree) {
	if (!tree) return;

	tree_delete_traversal(tree->root);
	free(tree);
}

void tree_delete_traversal(Node *ptr) {
	if (!ptr) return;

	tree_delete_traversal(ptr->left);
	tree_delete_traversal(ptr->right);
	node_delete(ptr);
}


int tree_insert(Tree *tree, char *key, char *info) {
	if (!tree) return 1;

	if (tree_find(tree, key)) {
		printf("[ERROR] Duplicate key!");
		return 2;
	}

	if (!tree->root) {
		Node *new_root= node_new(NULL, NULL, NULL, key, info);
		if (!new_root) return 1;

		tree->root = new_root;
	} else {
		Node *parent = tree_find_target_parent(tree, key);

        //printf("Target parent: %s\n", parent->key);
		Node *new_node = node_new(parent, NULL, NULL, key, info);
		if (!new_node) return 1;

		int cmp = strcmp(key, parent->key);
		if (cmp < 0)
			parent->left = new_node;
		else
			parent->right = new_node;
	}

	return 0;
}


int tree_remove(Tree *tree, char *key) {
	printf("[INFO] tree_remove() was not implemented!");
	// Node *ptr = tree_find(tree, key);
	// if (!node) return 2;

	// if (tree->root == ptr) {
	// 	tree->root = NULL;
	// 	node_delete(ptr);
	// }

	// if (ptr->left && ptr->right) {

	// }
	// else if (!ptr->left && !ptr->right) {
	// 	if (node_side(ptr) < 0)
	// 		ptr->parent->left = NULL;
	// 	else
	// 		ptr->parent->right = NULL;
	// } else {
	// 	if (ptr->left) {
	// 		if(node_side(ptr)) {

	// 		}


	// 	} else {

	// 	}
	// }
	return 0;
}


const Node* tree_find(Tree *tree, char *key) {
	if (!tree) return NULL;

	Node *ptr = tree->root;
	while (ptr) {
		int cmp = strcmp(key, ptr->key);
		if (cmp == 0)
            return ptr;
		else if (cmp < 0)
			ptr = ptr->left;
		else
			ptr = ptr->right;
	}

	return NULL;
}


const Node* tree_find_by_max_key(Tree *tree) {
	if (!tree) return NULL;

	Node *ptr = tree_find_max(tree->root);
	if (!ptr) {
		return NULL;
	}

	return ptr;
}

void tree_print(Tree *tree) {
	if (!tree) return;
    printf("(Key, Info)\n");
	tree_print_traversal(tree->root);
}

void tree_print_traversal(Node *ptr) {
	if (!ptr) return;

	tree_print_traversal(ptr->right);
	printf("(\"%s\", \"%s\")\n", ptr->key, ptr->info);
	tree_print_traversal(ptr->left);
}


void tree_draw(Tree *tree) {
	tree_draw_traversal(tree->root, 0);
}

void tree_draw_traversal(Node *ptr, int offset) {
    if (!ptr) return;

    tree_draw_traversal(ptr->right, offset + 1);

    for(int i = 0; i < offset; i++) {
        printf("\t\t");
    }
    printf("(\"%s\", \"%s\")\n\n\n", ptr->key, ptr->info);

    tree_draw_traversal(ptr->left, offset + 1);
}

int tree_load(char *filename) {
	return 0;
}


Node* tree_find_target_parent(Tree *tree, char *key) {
	Node *parent = NULL;
	Node *ptr    = tree->root;

	while (ptr) {
		parent = ptr;

		int cmp = strcmp(key, ptr->key);
		if (cmp < 0)
			ptr = ptr->left;
		else
			ptr = ptr->right;
	}

	return parent;
}


Node* tree_find_min(Node *ptr) {
	if (!ptr) return NULL;

	while (ptr->left) {
		ptr = ptr->left;
	}

	return ptr;
}


Node* tree_find_max(Node *ptr) {
	if (!ptr) return NULL;

	while (ptr->right) {
		ptr = ptr->right;
	}

	return ptr;
}


Node* tree_find_next(Node *ptr) {
	/*if (!ptr) return NULL;

	Node *parent;
	if (ptr->right) return tree_find_min(ptr->right);
	else {
		parent = ptr->parent;
		while (parent && (parent->right == ptr)) {
			ptr = parent;
			parent = parent->parent;
		}
	}

	return parent;*/
	return NULL;
}


int node_side(Node *ptr) {
	if(!ptr) return 0;
	if(!ptr->parent) return 0;
	if (ptr->parent->left == ptr)
		return -1;
	else
		return 1;
}

