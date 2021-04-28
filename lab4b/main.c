// AVL-tree
#include <stdlib.h>

#include "tree.h"
#include "menu.h"

const int DEBUG = 1;

int main(int argc, const char** argv) {
	Node *tree = NULL;

	tree_insert(&tree, 4, 4, "str");
	tree_insert(&tree, 2, 2, "str");
	tree_insert(&tree, 1, 1, "str");
	tree_insert(&tree, 3, 3, "str");
	tree_insert(&tree, 6, 6, "str");

	/*
	tree_insert(&tree, 7, 7.7, "str");
	tree_insert(&tree, 1, 1.1, "str");
	tree_insert(&tree, 4, 4.4, "str");
	tree_insert(&tree, 13, 13.13, "str");
	tree_insert(&tree, 9, 9.9, "str");
	tree_insert(&tree, 11, 11.11, "str");
	tree_insert(&tree, 6, 6.6, "str");*/
	//tree_insert(&tree, 8, 8.8, "str");
	//tree_insert(&tree, 2, 2.2, "str");
	//tree_insert(&tree, 5, 5.5, "str");
	//tree_insert(&tree, 3, 3.3, "str");
	//tree_insert(&tree, 10, 10.10, "str");
	//tree_insert(&tree, 12, 12.12, "str");

	tree_draw(tree, 0);

	start(&tree);

	tree_delete(tree);
	return 0;
}
