#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <limits.h>

#include "graph.h"

const int INF = INT_MAX;

//--------/ Edge /-------------------------------------------------------------
Edge* edge_new(int w, Node *near) {
	Edge *edge = (Edge*)calloc(1, sizeof(Edge));
	if (!edge) return NULL;

	edge->w = w;
	edge->c = w;
	edge->f = 0;

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

	node->i = 0;

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

	graph->size = 0;
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

	graph->size++;
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


			// remove all incoming edges firstly
			Node *cur_node = graph->list;
			while (cur_node) {
				node_remove_edge(cur_node, cur);
				cur_node = cur_node->next;
			}
			// then remove node with all outcoming edges
			node_delete(cur);
			graph->size++;
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



Node* graph_find(Graph *graph, char *key) {
	Node *cur = graph->list;
	while (cur) {
		if (strcmp(cur->key, key) == 0)
			break;
		cur = cur->next;
	}

	return cur;
}



//--------/ Graph Algos /------------------------------------------------------
int graph_dfs(Graph *graph, char *key_1, char *key_2) {
	// init
	graph_numerate(graph);
	int *colors  = (int*)calloc(graph->size, sizeof(int));
	Node **prevs = (Node**)calloc(graph->size, sizeof(Node*));


	Node *cur = graph_find(graph, key_1);
	if (!cur) return KEY_NOT_FOUND;

	Node *res = graph_dfs_visit(cur, key_2, colors, prevs);
	graph_print_path(res, prevs);
	printf("\n");

	free(colors);
	free(prevs);

	if (!res) return PATH_NOT_FOUND;
	return OK;
}


Node* graph_dfs_visit(Node* u, char *key_2, int *colors, Node **prevs) {
	colors[u->i] = BLACK;
	if (DEBUG) printf("\"%s\"\n", u->key);

	if (strcmp(u->key, key_2) == 0) return u;

	Edge *cur_edge = u->list;
	while (cur_edge) {
		Node *v = cur_edge->near;

		if (colors[v->i] == WHITE) {
			prevs[v->i] = u;

			Node *res = graph_dfs_visit(v, key_2, colors, prevs);
			if (res) return res;
		}

		cur_edge = cur_edge->next;
	}

	return NULL;
}



int graph_bf(Graph *graph, char *key_1, char *key_2) {
	// init
	graph_numerate(graph);
	int *dists  = (int*)calloc(graph->size, sizeof(int));
	Node **prevs = (Node**)calloc(graph->size, sizeof(Node*));

	for (int i = 0; i < graph->size; i++) {
		dists[i] = INF;
		prevs[i] = NULL;
	}

	Node *start = graph_find(graph, key_1);
	if (!start) return KEY_NOT_FOUND;

	Node *finish = graph_find(graph, key_2);
	if (!finish) return KEY_NOT_FOUND;

	dists[start->i] = 0;

	// from q to V - 1  - ???
	for (int i = 1; i <= graph->size; i++) {
		Node *cur_node = graph->list;
		while (cur_node) {
			int ui = cur_node->i;

			Edge *cur_edge = cur_node->list;
			while (cur_edge) {
				int vi = cur_edge->near->i;
				int w  = cur_edge->w;

				if (dists[vi] > dists[ui] + w) {
					dists[vi] = dists[ui] + w;
					prevs[vi] = dists[ui];
				}

				cur_edge = cur_edge->next;
			}

			cur_node = cur_node->next;
		}
	}


	// check negative loop
	Node *cur_node = graph->list;
	while (cur_node) {
		int ui = cur_node->i;

		Edge *cur_edge = cur_node->list;
		while (cur_edge) {
			int vi = cur_edge->near->i;
			int w  = cur_edge->w;

			if (dists[vi] > dists[ui] + w) {
				return NEGATIVE_LOOP_FOUND;
			}

			cur_edge = cur_edge->next;
		}

		cur_node = cur_node->next;
	}

	graph_print_path(finish, prevs);

	if (prevs[finish->i] == INF) return PATH_NOT_FOUND;
	return OK;
}


Graph* graph_rn(Graph *graph, char *key_1, char *key_2) {

}




void graph_numerate(Graph *graph) {
	Node *cur = graph->list;
	int i = 0;

	while (cur) {
		cur->i = i;
		i++;
		cur = cur->next;
	}
}


void graph_print_path(Node *node, Node **prevs) {
	if (!node) return;
	else graph_print_path(prevs[node->i], prevs);
	printf(" -> \"%s\"", node->key);
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
	printf("/----- DOT BEGIN -----/\n");
	printf("digraph G {\n");

	Node *cur_node = graph->list;
	while (cur_node) {
		Edge *cur_edge = cur_node->list;
		printf("\"%s\";\n", cur_node->key);

		while (cur_edge) {
			printf("\"%s\" -> \"%s\";\n", cur_node->key, cur_edge->near->key);

			cur_edge = cur_edge->next;
		}

		cur_node = cur_node->next;
	}

   	printf("}\n");
	printf("/------ DOT END  -----/\n");
}



Graph* graph_load() {

}


int graph_save(Graph *graph) {

}


int graph_random(Graph *graph, int size) {

}
