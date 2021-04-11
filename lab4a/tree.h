#ifndef	__TREE_H
#define __TREE_H

typedef struct Node {
	struct Node *parent;
	struct Node *left;
	struct Node *right;
	char *key; //unique
	char *info;
} Node;

typedef struct Tree {
	struct Node *root;
	int h; //height
	int n; //number of elements
} Tree;

Node* node_new(Node *parent, Node *left, Node *right, char *key, char *info);
void node_delete(Node *node);

Tree* tree_new();
void tree_delete(Tree *tree);
void tree_delete_traversal(Node *ptr);

int tree_insert(Tree *tree, char *key, char *info);
int tree_remove(Tree *tree, char *key);
const Node* tree_find(Tree *tree, char *key); //doesn't return a copy!
const Node* tree_find_by_max_key(Tree *tree); //returns element by max key

void tree_print(Tree *tree);
void tree_print_traversal(Node *ptr);
void tree_draw(Tree *tree);
void tree_draw_traversal(Node *ptr, int offset);
void tree_make_graphviz(Tree *tree);
void tree_make_graphviz_traversal(Node *ptr);

int tree_load(char *filename);

Node* tree_find_target_parent(Tree *tree, char *key);
Node* tree_find_min(Node *ptr);
Node* tree_find_max(Node *ptr);
Node* tree_find_next(Node *ptr);

int node_side(Node *ptr);

// + time




#endif
