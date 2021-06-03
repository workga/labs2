#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "tree.h"



//--------/ Info /-------------------------------------------------------------
Info* info_new(long int pos) {
	Info *info = (Info*)calloc(1, sizeof(Info));
	if (!info) return NULL;

	info->pos = pos;
	info->next = NULL;

	return info;
}


void info_delete(Info *info) {
	free(info);
}



//--------/ Node /-------------------------------------------------------------
Node* node_new(Node *parent, Node *left, Node *right, unsigned int key, Info *info) {
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

	if (prev) prev->next = NULL;
	else node->info = NULL;

	info_delete(cur);

	node->len--;

	return node->len;
}


int node_side(Node *node) {
    if (!node) return 0;
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


int node_bfactor(Node *node) {
	if (!node) return 0;
	return node_height(node->right) - node_height(node->left);
}


int node_fix_height(Node *node) {
    if (!node) return 0;
	int h_left   = node_height(node->left);
	int h_right  = node_height(node->right);
	node->h = (h_left > h_right ? h_left : h_right) + 1;
	return node->h;
}


Node* node_balance(Node *node) {
    if (!node) {
        if (DEBUG) printf("[DEBUG] Attemption to balance NULL");
        return NULL;
    }
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
		if (DEBUG) printf("[DEBUG] No need to balance node with key %d\n", node->key);
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


Node* tree_find(Node *node, unsigned int key) {
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


Node* tree_find_next(Node *node) {
	if (!node) return NULL;

	if (node->right) return tree_find_min(node->right);

	while (node->parent && node_side(node) > 0) {
		node = node->parent;
	}

	return node->parent; // maybe NULL
}


Node* tree_find_target_parent(Node *node, unsigned int key) {
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


const Info* tree_find_info(Node *node, unsigned int key, int num) {
	Node *found_node = tree_find(node, key);
	if (!found_node) return NULL;

	Info *info = node_get_info(found_node, num);
	return info;
}


const Info* tree_find_min_greater(Node *node, unsigned int key) {
	if (!node) return NULL;

	Node *cur = node;
	Node *prev = NULL;

	while (cur) {
		prev = cur;
		if (key < cur->key) {
			cur = cur->left;
		} else if (key > cur->key) {
			cur = cur->right;
		}
		else {
			Node *next_node = tree_find_next(cur);
			if (!next_node) return NULL;
			else return next_node->info;
		}
	}

	if (key < prev->key)
		return prev->info;
	else {
		Node *next_node = tree_find_next(prev);
		if (!next_node) return NULL;
		else return next_node->info;
	}
}


int tree_insert(Node **tree, unsigned int key, long int pos) {
	Info *info = info_new(pos);
	if (!info) return 1;

	// if key exists
	Node* found_node = tree_find(*tree, key);
	if (found_node) {
		//node_push_front(found_node, info);
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

	if (!p || node_bfactor(p) == 0) return 0; //height didn't change


	// only if (node_bfactor(p) == 2 or -2)

	// node_balance(p) doesn't change nodes above p
	int side = node_side(p);
    if (side < 0)
        p->parent->left = node_balance(p);
	else if (side > 0)
        p->parent->right = node_balance(p);
    else
    	*tree = node_balance(p);

	return 0;
}


int tree_remove(Node **tree, unsigned int key) {
	Node* found_node = tree_find(*tree, key);
	if (!found_node) return 1;

	if (node_remove_back(found_node)) return 0; // if found_node->len remains > 0

	// main case: delete node because its len == 0
    Node *x;
    Node *p;
    int old_h;
	if (found_node->left && found_node->right) { // found_node has 2 children
		// extract next_node from tree
		Node *next_node  = tree_find_min(found_node->right);
		Node *child_node = next_node->right;

		// for following balance
		x = child_node;
		p = next_node->parent;
		old_h = node_height(p);
		if (p == found_node) p = next_node;

		int side = node_side(next_node);
		if (side < 0)
			next_node->parent->left = child_node;
		else
			next_node->parent->right = child_node;

		if (child_node) {
			child_node->parent = next_node->parent;
		}

		// replace found_node with next_node
        next_node->left  = found_node->left;
        next_node->right = found_node->right;
        if (found_node->left) found_node->left->parent = next_node;
        if (found_node->right)found_node->right->parent = next_node;

        next_node->parent = found_node->parent;

		side = node_side(found_node);
		if (side < 0)
			found_node->parent->left = next_node;
		else if (side > 0)
			found_node->parent->right = next_node;
		else // if it was root
			*tree = next_node;

        node_delete(found_node); // is it clean?
	} else { // found_node has less than 2 children
		Node *child_node;
		if (found_node->left)
			child_node = found_node->left;
		else if (found_node->right)
			child_node = found_node->right;
		else
			child_node = NULL;

        // for following balance
        x = child_node;
		p = found_node->parent;
		old_h = node_height(p);


		if (child_node)
			child_node->parent = found_node->parent;

		int side = node_side(found_node);
		if (side < 0)
			found_node->parent->left  = child_node;
		else if (side > 0)
			found_node->parent->right = child_node;
		else
			*tree = child_node;

		node_delete(found_node);
	}

    if (DEBUG) {
        printf("[DEBUG] Before rebalancing:\n");
        tree_draw(*tree, 0);
    }
	//balance
    while (p && (node_fix_height(p) != old_h || node_bfactor(p) == 2 || node_bfactor(p) == -2)) {

        int side = node_side(p);
        if (side < 0)
            p->parent->left = node_balance(p);
        else if (side > 0)
            p->parent->right = node_balance(p);
        else
            *tree = node_balance(p);

        x = p;
        p = p->parent;
        old_h = node_height(p);
    }

    if (DEBUG) {
        printf("[DEBUG] After rebalancing:\n");
        tree_draw(*tree, 0);
    }

	return 0;
}



//--------/ Tree Utils /-------------------------------------------------------------
void tree_draw(Node *node, int offset) {
	if (!node) return;
	if (offset == 0) printf("\n");


    tree_draw(node->right, offset + 1);


    for(int i = 0; i < offset; i++) {
        printf("\t\t");
    }
    char c;
    int side = node_side(node);
    if (side < 0) c = '\\';
    else if (side > 0) c = '/';
    else c = ' ';
    if (node->info) printf("%c (k=%d, r=%d : [%ld])\n", c, node->key, node->len, node->info->pos);
    else printf("null info\n");


    tree_draw(node->left, offset + 1);
}
