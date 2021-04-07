#include "tree.h"
#include "menu.h"

int main(int argc, const char** argv) {
	Tree *tree = tree_new();
	tree_insert(tree, "f", "info f");
	tree_insert(tree, "e", "info e");
	tree_insert(tree, "i", "info i");
	tree_insert(tree, "a", "info a");
	tree_insert(tree, "c", "info c");
	tree_insert(tree, "b", "info b");
	tree_insert(tree, "g", "info g");
	tree_insert(tree, "h", "info h");
	tree_insert(tree, "d", "info d");
	start(tree);

	tree_delete(tree);
	return 0;
}
