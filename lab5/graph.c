#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "graph.h"



//--------/ Edge /-------------------------------------------------------------
Edge* edge_new(int w, Node *near) {
	Edge *edge = (Edge*)calloc(1, sizeof(Edge));
	if (!edge) return NULL;

	edge->w = w;
	edge->near = near;
	edge->next = NULL;

	return edge;
}


void edge_delete(Edge *edge) {
	free(edge);
}



//--------/ Node /-------------------------------------------------------------
Node* node_new(char *key, double x, double y) {
	Node *node = (Node*)calloc(1, sizeof(Node));
	if (!node) return NULL;

	node->key = (char*)calloc((int)strlen(key) + 1, sizeof(char));
	if (!node->key) {
		free(node);
		return NULL;
	}


	strcpy(node->key, key);
	node->x = x;
	node->y = y;
	node->list = NULL;
	node->next = NULL;

	return node;
}


void node_delete(Node *node) {
	Edge *cur_edge  = node->list;
	Edge *prev_edge = NULL;

	while (cur_edge) {
		prev_edge = cur_edge;
		cur_edge = cur_edge->next;
		edge_delete(prev_edge);
	}

	free(node->key);
	free(node);
}


int node_push_edge(Node *node, Node *near, int w) {
	// unordered
	Edge *cur  = node->list;
	Edge *prev = NULL;

	while (cur) {
		if (cur->near == near) return DUPLICATE_EDGE;
		prev = cur;
		cur  = cur->next;
	}

	Edge *edge = edge_new(w, near);
	if (!edge) return OUT_OF_MEMORY;

	if (prev)
		prev->next  = edge;
	else
		node->list = edge;

	return 0;
}


int node_remove_edge(Node *node, Node *near) {
	// unordered
	Edge *cur  = node->list;
	Edge *prev = NULL;

	while (cur) {
		if (cur->near == near) {
			if (prev)
				prev->next = cur->next;
			else
				node->list = cur->next;

			edge_delete(cur);

			return OK;
		}

		prev = cur;
		cur  = cur->next;
	}

	return EDGE_NOT_FOUND;
}



//--------/ Graph Basic /------------------------------------------------------
Graph* graph_new() {
	Graph *graph = (Graph*)calloc(1, sizeof(Graph));
	if (!graph) return NULL;

	graph->list = NULL;

	return graph;
}


void graph_delete(Graph *graph) {
	Node *cur  = graph->list;
	Node *prev = NULL;

	while (cur) {
		prev = cur;
		cur = cur->next;
		node_delete( prev);
	}

	free(graph);
}


int graph_insert_node(Graph *graph, char *key, double x, double y) {
	Node *cur  = graph->list;
	Node *prev = NULL;

	while (cur) {
		if (strcmp(cur->key, key) == 0) return DUPLICATE_KEY;
		prev = cur;
		cur  = cur->next;
	}

	Node *node = node_new(key, x, y);
	if (!node) return OUT_OF_MEMORY;

	if (prev)
		prev->next  = node;
	else
		graph->list = node;

	return OK;
}


int graph_remove_node(Graph *graph, char *key) {
	Node *cur  = graph->list;
	Node *prev = NULL;

	while (cur) {
		if (strcmp(cur->key, key) == 0) {
			if (prev)
				prev->next  = cur->next;
			else
				graph->list = cur->next;


			Node *cur_node = graph->list;
			while (cur_node) {
				node_remove_edge(cur_node, cur);
				cur_node = cur_node->next;
			}
			node_delete(cur);

			return OK;
		}

		prev = cur;
		cur  = cur->next;
	}

	return KEY_NOT_FOUND;
}


int graph_insert_edge(Graph *graph, char *key_1, char *key_2, int w) {
	Node *cur  = graph->list;
	Node *node = NULL;
	Node *near = NULL;

	if (strcmp(key_1, key_2) == 0) return INVALID_INPUT;

	while (cur) {
		if (strcmp(cur->key, key_1) == 0) {
			node = cur;
			if (near) break;
		} else if (strcmp(cur->key, key_2) == 0) {
			near = cur;
			if (node) break;
		}

		cur  = cur->next;
	}

	if (node && near)
		return node_push_edge(node, near, w);
	else
		return KEY_NOT_FOUND;
}


int graph_remove_edge(Graph *graph, char *key_1, char *key_2) {
	Node *cur  = graph->list;
	Node *node = NULL;
	Node *near = NULL;

	if (strcmp(key_1, key_2) == 0) return INVALID_INPUT;

	while (cur) {
		if (strcmp(cur->key, key_1) == 0) {
			node = cur;
			if (near) break;
		} else if (strcmp(cur->key, key_2) == 0) {
			near = cur;
			if (node) break;
		}

		cur  = cur->next;
	}

	if (node && near)
		return node_remove_edge(node, near);
	else
		return KEY_NOT_FOUND;
}



//--------/ Graph Algos /------------------------------------------------------
void graph_dfs(Graph *graph, char *key_1, char *key_2) {

}


void graph_bf(Graph *graph, char *key_1, char *key_2) {

}


Graph* graph_rn(Graph *graph, char *key_1, char *key_2) {

}


//auxiliary functions


//--------/ Graph Utils /------------------------------------------------------
void graph_print(Graph *graph) {
	Node *cur_node = graph->list;

	while (cur_node) {
		printf("(key: \"%s\", x: %lf, y: %lf)", cur_node->key, cur_node->x, cur_node->y);

		Edge *cur_edge = cur_node->list;

		while (cur_edge) {
			printf(" -> (key:  \"%s\", w: %d)", cur_edge->near->key, cur_edge->w);

			cur_edge = cur_edge->next;
		}
		printf("\n");

		cur_node = cur_node->next;
	}
}


void graph_make_graphviz(Graph *graph) {

}



Graph* graph_load() {

}


int graph_save(Graph *graph) {

}


int graph_random(Graph *graph, int size) {

}
