#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <limits.h>
#include <time.h>
#include <math.h>

#include "graph.h"

const int INF = INT_MAX;
const int MINUS_INF = INT_MIN;

const int RAND_MAX_XY	 = 100;
const int RAND_MAX_W	 = 100;
const int RAND_MAX_C	 = 100;
const double RAND_MAX_EDGES = 0.9;
const double RAND_MIN_EDGES = 0.1;


//--------/ Edge /-------------------------------------------------------------
Edge* edge_new(Node *near, int w, int c, int f) {
	Edge *edge = (Edge*)calloc(1, sizeof(Edge));
	if (!edge) return NULL;

	edge->w = w;
	edge->c = c;
	edge->f = f;

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
	node->deg = 0;

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


int node_push_edge(Node *node, Node *near, int w, int c, int f) {
	// unordered
	Edge *cur  = node->list;
	Edge *prev = NULL;

	while (cur) {
		if (cur->near == near) return DUPLICATE_EDGE;
		prev = cur;
		cur  = cur->next;
	}

	Edge *edge = edge_new(near, w, c, f);
	if (!edge) return OUT_OF_MEMORY;

	if (prev)
		prev->next  = edge;
	else
		node->list = edge;

	node->deg++;

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
			node->deg--;
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
	graph_remove_all(graph);
	free(graph);
}


void graph_remove_all(Graph *graph) {
	Node *cur  = graph->list;
	Node *prev = NULL;

	while (cur) {
		prev = cur;
		cur = cur->next;
		node_delete(prev);
		graph->size--;
	}
	graph->list = NULL;
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


int graph_insert_edge(Graph *graph, char *key_1, char *key_2, int w, int c) {
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
		return node_push_edge(node, near, w, c, 0);
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

	Node *start = graph_find(graph, key_1);
	if (!start) return KEY_NOT_FOUND;

	Node *finish = graph_find(graph, key_2);
	if (!finish) return KEY_NOT_FOUND;

	if (start == finish) return INVALID_INPUT;

	// init
	graph_numerate(graph);
	int *colors  = (int*)calloc(graph->size, sizeof(int));
	Node **prevs = (Node**)calloc(graph->size, sizeof(Node*));


	finish = graph_dfs_visit(start, finish, colors, prevs);
	graph_print_path(graph, finish, prevs);

	free(colors);
	free(prevs);

	if (!finish) return PATH_NOT_FOUND;
	return OK;
}


Node* graph_dfs_visit(Node* u, Node *finish, int *colors, Node **prevs) {
	colors[u->i] = BLACK;
	//if (DEBUG) printf("\"%s\"\n", u->key);

	if (u == finish) return u;

	Edge *cur_edge = u->list;
	while (cur_edge) {
		Node *v = cur_edge->near;

		if (colors[v->i] == WHITE) {
			prevs[v->i] = u;

			Node *res = graph_dfs_visit(v, finish, colors, prevs);
			if (res) return res;
		}

		cur_edge = cur_edge->next;
	}

	return NULL;
}



int graph_bf(Graph *graph, char *key_1, char *key_2) {

	Node *start = graph_find(graph, key_1);
	if (!start) return KEY_NOT_FOUND;

	Node *finish = graph_find(graph, key_2);
	if (!finish) return KEY_NOT_FOUND;

	if (start == finish) return INVALID_INPUT;


	// init
	graph_numerate(graph);
	int *dists  = (int*)calloc(graph->size, sizeof(int));
	Node **prevs = (Node**)calloc(graph->size, sizeof(Node*));

	for (int i = 0; i < graph->size; i++) {
		dists[i] = INF;
		prevs[i] = NULL;
	}
	dists[start->i] = 0;

	// from 1 to V - 1  - ???
	for (int i = 1; i <= graph->size - 1; i++) {
		Node *cur_node = graph->list;
		while (cur_node) {
			int ui = cur_node->i;

			Edge *cur_edge = cur_node->list;
			while (cur_edge) {
				int vi = cur_edge->near->i;
				int w  = cur_edge->w;

				if ((dists[ui] != INF) && (dists[vi] > dists[ui] + w)) {
					dists[vi] = dists[ui] + w;
					prevs[vi] = cur_node;
				}

				cur_edge = cur_edge->next;
			}

			cur_node = cur_node->next;
		}
	}


	// check negative loop:
	// a) only for edges in path of prevs
	// Node *cur = finish;
	// while (prevs[cur->i]) {
	// 	Node *u = prevs[cur->i];
	// 	Node *v = cur;
	// 	int w;
	// 	Edge *cur_edge = u->list;
	// 	while (cur_edge) {
	// 		if (cur_edge->near == v) {
	// 			w = cur_edge->w;
	// 			break;
	// 		}
	// 		cur_edge = cur_edge->next;
	// 	}

	// 	int ui = u->i;
	// 	int vi = v->i;

	// 	if ((dists[ui] != INF) && (dists[vi] > dists[ui] + w)) {
	// 			free(dists);
	// 			free(prevs);
	// 			return NEGATIVE_LOOP_FOUND;
	// 	}

	// 	cur = prevs[cur->i];
	// }


	// b) original solution
	Node *cur_node = graph->list;
	while (cur_node) {
		int ui = cur_node->i;

		Edge *cur_edge = cur_node->list;
		while (cur_edge) {
			int vi = cur_edge->near->i;
			int w  = cur_edge->w;

			if ((dists[ui] != INF) && (dists[vi] > dists[ui] + w)) {
				free(dists);
				free(prevs);
				return NEGATIVE_LOOP_FOUND;
			}

			cur_edge = cur_edge->next;
		}

		cur_node = cur_node->next;
	}


	if (dists[finish->i] == INF) {
		free(dists);
		free(prevs);
		return PATH_NOT_FOUND;
	}
	else {
		graph_print_path(graph, finish, prevs);
		free(dists);
		free(prevs);
		return OK;
	}
}


int graph_rn_max_flow(Graph *graph, char *key_1, char *key_2) {
	// set flow to zero
	Node *cur = graph->list;
	while (cur) {
		Edge *edge = cur->list;
		while (edge) {
			edge->f = 0;
			edge = edge->next;
		}
		cur = cur->next;
	}


	// create Gf
	Graph *graph_f = graph_rn(graph);

	Node *start = graph_find(graph_f, key_1);
	if (!start) {
		graph_delete(graph_f);
		return KEY_NOT_FOUND;
	}

	Node *finish = graph_find(graph_f, key_2);
	if (!finish) {
		graph_delete(graph_f);
		return KEY_NOT_FOUND;
	}

	if (start == finish){
		graph_delete(graph_f);
		return KEY_NOT_FOUND;
	}


	graph_numerate(graph_f);
	int *colors_f  = (int*)calloc(graph_f->size, sizeof(int));
	Node **prevs_f = (Node**)calloc(graph_f->size, sizeof(Node*));


	// while aug path exists
	while ((finish = graph_dfs_visit(start, finish, colors_f, prevs_f)) != NULL) {
		// find min cf in path
		int cf_min = get_edge(prevs_f[finish->i], finish)->c;
		Node *cur = finish;
		while (prevs_f[cur->i]) {
			int cur_c = get_edge(prevs_f[cur->i], cur)->c;
			cf_min = ((cf_min < cur_c) ? cf_min : cur_c);

			cur = prevs_f[cur->i];
		}
		if (DEBUG) {
			printf("[PATH] cf_min: %d\n", cf_min);
			graph_print_path(graph_f, finish, prevs_f);
		}

		// increase f along the path
		Node *cur_f = finish;
		while (prevs_f[cur_f->i]) {
			Node *cur   = graph_find(graph, cur_f->key);
			Node *prev  = graph_find(graph, prevs_f[cur_f->i]->key);

			int f = get_flow(prev, cur);
			set_flow(prev, cur, f + cf_min);

			cur_f = prevs_f[cur_f->i];
		}


		// clear
		for (int i = 0; i < graph_f->size; i++) {
			colors_f[i] = WHITE;
			prevs_f[i]  = NULL;
		}

		graph_delete(graph_f);
		graph_f = graph_rn(graph);

        graph_numerate(graph_f);

		start = graph_find(graph_f, key_1);
		finish = graph_find(graph_f, key_2);
	}
	// here we have max flow and corresponding Gf
	graph_make_graphviz(graph_f);
	graph_make_graphviz(graph);


	free(colors_f);
	free(prevs_f);

	graph_delete(graph_f);

	return OK;
}


Graph* graph_rn(Graph *graph) {
	Graph *graph_f = graph_new();


	// insert nodes
	Node* cur = graph->list;
	while (cur) {
		graph_insert_node(graph_f, cur->key, cur->x, cur->y);
		cur = cur->next;
	}


	// insert edges with capacities
	Node *u = graph->list;
	while (u) {
		Edge *edge = u->list;
		while (edge) {
			Node *v = edge->near;
			// here we have: u, v, edge


			Node *uf = graph_find(graph_f, u->key);
			Node *vf = graph_find(graph_f, v->key);

			int cf_uv = edge->c - edge->f;
			if (cf_uv > 0) set_capacity(uf, vf, cf_uv);

			int cf_vu = get_capacity(v, u) + edge->f;
			if (cf_vu > 0) set_capacity(vf, uf, cf_vu);

			edge = edge->next;
		}

		u = u->next;
	}

	//graph_make_graphviz(graph_f);
	return graph_f;
}


Edge* get_edge(Node *u, Node *v) {
	if (!u) return NULL;
	if (!v) return NULL;

	Edge *cur_edge = u->list;
	while (cur_edge) {
		if (cur_edge->near == v) return cur_edge;
		cur_edge = cur_edge->next;
	}

	return NULL;
}


int get_capacity(Node *u, Node *v) {
	Edge *edge = get_edge(u, v);
	if (edge) return edge->c;
	else return 0; // if no edge - c == 0
}


void set_capacity(Node *u, Node *v, int c) {
	Edge *edge = get_edge(u, v);
	if (edge) edge->c = c;
	else if (c > 0) node_push_edge(u, v, 0, c, (-1)*get_flow(v, u)); // if no edge - add edge
}


int get_flow(Node *u, Node *v) {
	Edge *edge = get_edge(u, v);
	if (edge) return edge->f;
	else {
		edge = get_edge(v, u);
		if (edge) return (-1)*edge->f;
		else return 0; // if no edges - no flow
	}
}


void set_flow(Node *u, Node *v, int f) {
	Edge *edge_uv = get_edge(u, v);
	Edge *edge_vu = get_edge(v, u);

	if (edge_uv) edge_uv->f = f;
	if (edge_vu) edge_vu->f = (-1)*f;
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


void graph_print_path(Graph *graph, Node *node, Node **prevs) {
	if (!node) return;

	Node **path = (Node**)calloc(graph->size, sizeof(Node*));
	int   *ws   = (int*)calloc(graph->size, sizeof(int));
	int   *cs   = (int*)calloc(graph->size, sizeof(int));

	int count = 0;
	Node *cur = node;
	while (cur) {
		cur = prevs[cur->i];
		count++;
	}

	int i = count - 1;
	cur = node;
	while (cur) {
		path[i] = cur;
		Edge *edge = get_edge(prevs[cur->i], cur);
		if (edge) {
			ws[i] = edge->w;
			cs[i] = edge->c;
		} else {
			ws[i] = INF;
			cs[i] = 0;
		}

		cur = prevs[cur->i];
		i--;
	}

	for (int i = 0; i < count - 1; i++) {
		printf("\"%s\" --(w:%d, c:%d)--> ", path[i]->key, ws[i+1], cs[i+1]);
	}
	printf("\"%s\"\n", path[count - 1]->key);

	free(path);
	free(ws);
	free(cs);
}

//auxiliary functions


//--------/ Graph Utils /------------------------------------------------------
void graph_print(Graph *graph) {
	Node *cur_node = graph->list;

	while (cur_node) {
		printf("(key: \"%s\", x: %lf, y: %lf)",
			   cur_node->key, cur_node->x, cur_node->y);

		Edge *cur_edge = cur_node->list;

		while (cur_edge) {
			printf(" -> (key:  \"%s\", w: %d, f: %d, c: %d)",
				   cur_edge->near->key, cur_edge->w, cur_edge->f, cur_edge->c);

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
			printf("\"%s\" -> \"%s\" [label = \"%d, %d/%d\"];\n",
				   cur_node->key, cur_edge->near->key, cur_edge->w, cur_edge->f, cur_edge->c);

			cur_edge = cur_edge->next;
		}

		cur_node = cur_node->next;
	}

   	printf("}\n");
	printf("/------ DOT END  -----/\n");
}



Graph* graph_load() {
	FILE *fp;
	fp = fopen("data.bin", "rb");
	if (!fp) return NULL;


	Graph *graph = graph_new();
	int size;
	fread(&size, sizeof(int), 1, fp);
	for (int i = 0; i < size; i++) {
		int len;
		fread(&len, sizeof(int), 1, fp);

		char *key = (char*)calloc(len + 1, sizeof(char));
		fread(key, sizeof(char), len, fp);
		key[len] = '\0';

		double x;
		double y;
		fread(&x, sizeof(double), 1, fp);
		printf("%d", i);
		fread(&y, sizeof(double), 1, fp);
        printf(" %d\n", i);

		graph_insert_node(graph, key, x, y);
		free(key);
	}

	Node *cur = graph->list;
	while (cur) {
		int deg;
		fread(&deg, sizeof(int), 1, fp);

		for (int j = 0; j < deg; j++) {
			int w;
			int c;

			fread(&w, sizeof(int), 1, fp);
			fread(&c, sizeof(int), 1, fp);


			int len;
			fread(&len, sizeof(int), 1, fp);

			char *key = (char*)calloc(len + 1, sizeof(char));
			fread(key, sizeof(char), len, fp);
			key[len] = '\0';

			graph_insert_edge(graph, cur->key, key, w, c);


			free(key);
		}

		cur = cur->next;
	}

	fclose(fp);


	return graph;
}


int graph_save(Graph *graph) {
	FILE *fp;
	fp = fopen("data.bin", "wb");
	if (!fp) return 1;


	fwrite(&(graph->size), sizeof(int), 1, fp);
	Node *cur = graph->list;
	while (cur) {
		int len = strlen(cur->key);
		fwrite(&len, sizeof(int), 1, fp);
		fwrite(cur->key, sizeof(char), len, fp);
		fwrite(&(cur->x), sizeof(double), 1, fp);
		fwrite(&(cur->y), sizeof(double), 1, fp);

		cur = cur->next;
	}

	cur = graph->list;
	while (cur) {
		fwrite(&(cur->deg), sizeof(int), 1, fp);

		Edge *edge = cur->list;
		while (edge) {
			fwrite(&(edge->w), sizeof(int), 1, fp); // w
			fwrite(&(edge->c), sizeof(int), 1, fp); // c
			int len = strlen(edge->near->key);
			fwrite(&len, sizeof(int), 1, fp);
			fwrite(edge->near->key, sizeof(char), len, fp);

			edge = edge->next;
		}

		cur = cur->next;
	}

	fclose(fp);
	return 0;
}


int graph_random(Graph *graph, int size) {
	graph_remove_all(graph);

	int e;
	srand(time(NULL));
	for (int i = 0; i < size; i++) {
		int x;
		int y;
		int len;
		char *key = NULL;
		do {
			free(key);
			x = rand() % RAND_MAX_XY;
			y = rand() % RAND_MAX_XY;
			len = calculate_len(size);
			key = random_str(len);
		} while ((e = graph_insert_node(graph, key, x, y)) != OK &&
				 (e != OUT_OF_MEMORY));
		free(key);

		if (e == OUT_OF_MEMORY) {
			graph_remove_all(graph);
			return OUT_OF_MEMORY; // here clear all graph
		}
	}


	int count = calculate_edges_count(size);
	for (int i = 0; i < count; i++) {
		int ui;
		int vi;
		char *key_1;
		char *key_2;
		int w;
		int c;

		do {
			ui = rand() % size;
			vi = rand() % size;

			key_1 = get_node_by_index(graph, ui)->key;
			key_2 = get_node_by_index(graph, vi)->key;

			w = rand() % (2*RAND_MAX_W + 1) - RAND_MAX_W;
			c = rand() % (RAND_MAX_C + 1);
		} while ((e = graph_insert_edge(graph, key_1, key_2, w, c)) != OK &&
				 (e != OUT_OF_MEMORY));
		if (e == OUT_OF_MEMORY) {
			graph_remove_all(graph);
			return OUT_OF_MEMORY;
		}
	}

	return OK;
}


int calculate_len(int size) {
	int len = (int)(log(100*size)/log(26.));
	return len;
}


int calculate_edges_count(int size) {
	int whole_count = (int)(size*(size - 1)*0.5);
	int min_count = (int)(whole_count*RAND_MIN_EDGES);
	int max_count = (int)(whole_count*RAND_MAX_EDGES);
	int count = rand() % (max_count - min_count) + min_count;
	return count;
}


Node* get_node_by_index(Graph *graph, int index) {
	if (index > graph->size - 1) return NULL;

	int cur_i = 0;
	Node *cur = graph->list;
	while (cur_i < index) {
		cur = cur->next;
		cur_i++;
	}

	return cur;
}


char* random_str(int len)
{
    char *str = (char*)calloc(len + 1, sizeof(char));
    if (!str) return NULL;

    char *cur = str;
    int m = 26; // 'z' + 1 - 'a' //122 + 1 - 97 = 26
    int d = 'a'; //97
    for (int i = 0; i < len; i++) {
        int r = rand()%m;
        *cur = d + r;
        cur++;
    }
    *cur = '\0';

    return str;
}

int edge_weight_by_nodes(Node *u, Node *v) {
	if (!u) return MINUS_INF;

	Edge *cur_edge = u->list;
	while (cur_edge) {
		if (cur_edge->near == v) return cur_edge->w;
		cur_edge = cur_edge->next;
	}

	return INF;
}
