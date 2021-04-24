#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "tree.h"



//--------/ Info /-------------------------------------------------------------
Info* info_new(float flt, char *str) {
	if (!str) return NULL;

	Info *info = (Info*)calloc(1, sizeof(Info));
	if (!info) return NULL;

	info->str = (char*)calloc((int)strlen(str) + 1, sizeof(char));
	if (!info->str) {
		free(info);
		return NULL;
	}

	//-------------------------------------------

	info->flt = flt;
	strcpy(info->str, str);

	return info;
}


void info_delete(Info *info) {
	free(info->str);
	free(info);
}



//--------/ Node /-------------------------------------------------------------
Node* node_new(Node *parent, Node *left, Node *right, int key, Info *info) {
	if (!key) return NULL;
	if (!info) return NULL;

	Node *node = (Node*)calloc(1, sizeof(Node));
	if (!node) return NULL;

	//-------------------------------------------
	node->key    = key;
	node->h      = 1;
	node->len    = 1;

	node->info   = info;

	node->parent = parent;
	node->left   = left;
	node->right  = right;

	return node;
}


void node_delete(Node *node) {
	Info *cur  = node->info;
	Info *prev = NULL;

	while (cur) {
		prev = cur;
		cur = cur->next;
		info_delete(prev);
	}

	free(node);
}


Info* node_get_info(Node *node, int num) {
	if (num > node->len) return NULL;

	Info *cur = node->info;
	int i_cur = node->len;
	while (i_cur != num) {
		cur = cur->next;
		i_cur--;
	}

	return cur;
}


void node_push_front(Node *node, Info *info) {
	info->next = node->info;
	node->info = info;

	node->len++;
}


int node_remove_back(Node *node) {
	Info *cur  = node->info;
	Info *prev = NULL;

	while (cur->next) {
		prev = cur;
		cur = cur->next;
	}

	prev->next = NULL;
	info_delete(cur);

	node->len--;

	return node->len;
}


int node_side(Node *node) {
	if(!node->parent) return 0;
	if (node->parent->left == node)
		return -1;
	else
		return 1;
}


int node_height(Node *node) {
	return node ? node->h : 0;
}




//--------/ AVL functions /----------------------------------------------------------
Node* node_rotate_left(Node *node) {
	Node *x = node;
	Node *y = x->right;
	Node *b = y->left;


	x->right = b;
	if (b) b->parent = x;

	y->left  = x;
	y->parent = x->parent;
	x->parent = y;

	node_fix_height(x);
	node_fix_height(y);

	return y;
}


Node* node_rotate_right(Node *node) {
	Node *x = node;
	Node *y = x->left;
	Node *b = y->right;


	x->left = b;
	if (b) b->parent = x;

	y->right  = x;
	y->parent = x->parent;
	x->parent = y;

	node_fix_height(x);
	node_fix_height(y);

	return y;
}


Node* node_big_rotate_left(Node **p_node) {
 //    tree_draw(*p_node, 0);
	// Node *x = *p_node;
	// Node *y = x->right;

	// node_rotate_right(&y);
 //    tree_draw(*p_node, 0);
 //    //node_rotate_left(&x);

	// return y;
}


Node* node_big_rotate_right(Node **p_node) {
	// Node *x = *p_node;
	// Node *y = x->left;

	// node_rotate_left(&y);
	// node_rotate_right(&x);

	// return y;
}


int node_bfactor(Node *node) {
	if (!node) return 0;
	return node_height(node->right) - node_height(node->left);
}


int node_fix_height(Node *node) {
	int h_left   = node_height(node->left);
	int h_right  = node_height(node->right);
	node->h = (h_left > h_right ? h_left : h_right) + 1;
	return node->h;
}


Node* node_balance(Node *node) {
	Node *p = node;
	node_fix_height(p);

	if (node_bfactor(p) == 2) {
		if(node_bfactor(p->right) < 0 )
			p->right = node_rotate_right(p->right);
		return node_rotate_left(p);
	}
	else if (node_bfactor(p) == -2) {
		if(node_bfactor(p->left) > 0 )
			p->left = node_rotate_left(p->left);
		return node_rotate_right(p);
	}
	else {
		if (DEBUG) printf("[DEBUG] No need to balance node with key %d", node->key);
		return p;
	}
}



//--------/ Tree Basic /-------------------------------------------------------------
void tree_delete(Node *tree) {
	if(!tree) return;

	tree_delete(tree->left);
	tree_delete(tree->right);
	node_delete(tree);
}


Node* tree_find(Node *node, int key) {
	if (!node) return NULL;

	while (node) {
		if (key < node->key)
			node = node->left;
		else if (key > node->key)
			node = node->right;
		else break;
	}

	return node;
}


Node* tree_find_min(Node *node) {
	if (!node) return NULL;

	while (node->left) {
		node = node->left;
	}

	return node;
}


Node* tree_find_target_parent(Node *node, int key) {
	Node *parent = NULL;
	Node *cur    = node;

	while (cur) {
		parent = cur;

		if (key < cur->key)
			cur = cur->left;
		else if (key > cur->key)
			cur = cur->right;
		else // error because key have found, never happens
			return NULL;
	}

	return parent;
}


const Info* tree_find_info(Node *node, int key, int num) {
	Node *found_node = tree_find(node, key);
	if (!found_node) return NULL;

	Info *info = node_get_info(found_node, num);
	return info;
}


const Info* tree_find_min_greater(Node *node, int key, int num) {
	//...
}


int tree_insert(Node **tree, int key, float flt, char *str) {
	Info *info = info_new(flt, str);
	if (!info) return 1;

	// if key exists
	Node* found_node = tree_find(*tree, key);
	if (found_node) {
		node_push_front(found_node, info);
		return 0;
	}


	// else if key doesn't exist
	Node *x = node_new(NULL, NULL, NULL, key, info);
	if (!x) {
		info_delete(info);
		return 1;
	}

	// if tree is empty
	if (*tree == NULL) {
		*tree = x;
		return 0;
	}

	// main case: key doesn't exist and tree is not empty
	Node *p = tree_find_target_parent(*tree, key);
	x->parent = p;
	if (key < p->key)
		p->left  = x;
	else
		p->right = x;


	// rebalance tree
	while (p && (node_bfactor(p) == 1 || node_bfactor(p) == -1)) {
		node_fix_height(p);

		x = p;
		p = p->parent;
	}
    //4???
	if (!p || node_bfactor(p) == 0) return 0; //height didn't change

    /*
    if (!p) return 0;

	if ((node_side(x) < 0 && node_bfactor(p) ==  1) ||
		(node_side(x) > 0 && node_bfactor(p) == -1) ) {
		//node_fix_height(p); //no need
		return 0;
	}*/

	// only if (node_bfactor(p) == 2 or -2)
	//Node *new_p = node_balance(p);

	// node_balance(p) doesn't change nodes above p
    if (node_side(p) < 0)
        p->parent->left = node_balance(p);
	else if (node_side(p) > 0)
        p->parent->right = node_balance(p);
    else
    	*tree = node_balance(p);



	return 0;
}


int tree_remove(Node **tree, int key) {
	Node* found_node = tree_find(*tree, key);
	if (!found_node) return 1;

	node_remove_back(found_node);
	if (found_node->len > 0) return 0;

	// main case: delete node because its len == 0
	//!!!
}



//--------/ Tree Utils /-------------------------------------------------------------
void tree_print(Node *node) {
    if (!node) return;
	tree_print(node->right);
	printf("(k=%d, r=%d : [%4.2f, \"%s\"])\n", node->key, node->len, node->info->flt, node->info->str);
	tree_print(node->left);
}


void tree_draw(Node *node, int offset) {
	if (!node) return;
	if (offset == 0) printf("\n");


    tree_draw(node->right, offset + 1);


    for(int i = 0; i < offset; i++) {
        printf("\t\t");
    }
    char c;
    if (node_side(node) < 0) c = '\\';
    else if (node_side(node) > 0) c = '/';
    else c = ' ';
    printf("%c (k=%d, r=%d : [%4.2f, \"%s\"])\n", c, node->key, node->len, node->info->flt, node->info->str);


    tree_draw(node->left, offset + 1);
}


void tree_make_graphviz(Node *node, int first) {
	if(!node) return;

	if (first) {
		printf("/----- DOT BEGIN -----/\n");
		printf("digraph G {\n");
	}
	//-------------------------------------------


	//----/ left subtree /-------------
    if (node->left) tree_make_graphviz(node->left, 0);
    else {
    	printf("\"(k=%d, r=%d : [%4.2f, \'%s\'])\" -> \"(k=%d, r=%d : [%4.2f, \'%s\'])_left\"\n",
    			node->key, node->len, node->info->flt, node->info->str,
    			node->key, node->len, node->info->flt, node->info->str);
    	printf("\"(k=%d, r=%d : [%4.2f, \'%s\'])_left\"[shape=point]\n",
    			node->key, node->len, node->info->flt, node->info->str);
   	}


   	//----/ current node /-------------
    if (node->parent) {
	    printf("\"(k=%d, r=%d : [%4.2f, \'%s\'])\" -> \"(k=%d, r=%d : [%4.2f, \'%s\'])\"\n",
	    	   node->parent->key, node->parent->len, node->parent->info->flt, node->parent->info->str,
	    	   node->key, node->len, node->info->flt, node->info->str);
    }


	//----/ right subtree /-------------
    if (node->right) tree_make_graphviz(node->right, 0);
    else {
    	printf("\"(k=%d, r=%d : [%4.2f, \'%s\'])\" -> \"(k=%d, r=%d : [%4.2f, \'%s\'])_right\"\n",
    			node->key, node->len, node->info->flt, node->info->str,
    			node->key, node->len, node->info->flt, node->info->str);
    	printf("\"(k=%d, r=%d : [%4.2f, \'%s\'])_right\"[shape=point]\n",
    			node->key, node->len, node->info->flt, node->info->str);
   	}


   	//-------------------------------------------
	if (first) {
		printf("}\n");
		printf("/------ DOT END  -----/\n");
	}

}


void tree_load(Node **tree) {
	//...
}
