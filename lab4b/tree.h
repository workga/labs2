#ifndef	__TREE_H
#define __TREE_H

const int DEBUG;

typedef struct Info {
	float flt;
	char *str;
	struct Info *next;
} Info;

typedef struct Node {
	int key;
	int h;
	int len;
	struct Info *info;
	struct Node *parent, *left, *right;
} Node;



//--------/ Info /-------------------------------------------------------------
Info* info_new(float flt, char *str);
void  info_delete(Info *info);



//--------/ Node /-------------------------------------------------------------
Node* node_new(Node *parent, Node *left, Node *right, int key, Info *info);
void  node_delete(Node *node);
Info* node_get_info(Node *node, int num);
void  node_push_front(Node *node, Info *info);
int   node_remove_back(Node *node);
int   node_side(Node *node);
int   node_height(Node *node);



//--------/ AVL functions /----------------------------------------------------------
Node* node_rotate_left(Node *node);
Node* node_rotate_right(Node *node);
int   node_bfactor(Node *node);
int   node_fix_height(Node *node);

Node* node_balance(Node *node);



//--------/ Tree Basic /-------------------------------------------------------------
void  tree_delete(Node *tree);

Node* tree_find(Node *node, int key);
Node* tree_find_min(Node *node);
Node* tree_find_next(Node *node);
Node* tree_find_target_parent(Node *node, int key);
const Info* tree_find_info(Node *node, int key, int num);
const Info* tree_find_min_greater(Node *node, int key);

int   tree_insert(Node **tree, int key, float flt, char *str);
int   tree_remove(Node **tree, int key);



//--------/ Tree Utils /-------------------------------------------------------------
void  tree_print(Node *node);
void  tree_print_range(Node *node, int key_min, int key_max);
void  tree_draw(Node *node, int offset);
void  tree_make_graphviz(Node *node, int first);

void  tree_load(Node **tree);


#endif
