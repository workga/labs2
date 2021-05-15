#ifndef __GRAPH_H
#define __GRAPH_H


const int DEBUG;
const int INF;


enum Error {
	OK,
	OUT_OF_MEMORY,
	DUPLICATE_KEY,
	DUPLICATE_EDGE,
	KEY_NOT_FOUND,
	EDGE_NOT_FOUND,
	INVALID_INPUT,
	END_OF_FILE,
	PATH_NOT_FOUND,
	NEGATIVE_LOOP_FOUND
};


enum Color {
	WHITE, // 0
	GREY,
	BLACK
};


//--------/ Structures /-------------------------------------------------------
struct Node;

typedef struct Edge {
	int w;
	int c;
	int f;

	struct Node *near;
	struct Edge *next;
} Edge;

typedef struct Node {
	char *key;
	double x, y;

	int i;

	struct Edge *list;
	struct Node *next; // next in graph's list
} Node;

typedef struct Graph {
	int size;
	struct Node *list;
} Graph;


//--------/ Edge /-------------------------------------------------------------
Edge*  edge_new(int w, Node *near);
void   edge_delete(Edge *edge);


//--------/ Node /-------------------------------------------------------------
Node*  node_new(char *key, double x, double y);
void   node_delete(Node *node);
int    node_push_edge(Node *node, Node *near, int w);
int    node_remove_edge(Node *node, Node *near);


//--------/ Graph Basic /------------------------------------------------------
Graph* graph_new();
void   graph_delete(Graph *graph);

int    graph_insert_node(Graph *graph, char *key, double x, double y);
int    graph_remove_node(Graph *graph, char *key);

int    graph_insert_edge(Graph *graph, char *key_1, char *key_2, int w);
int    graph_remove_edge(Graph *graph, char *key_1, char *key_2);

Node*  graph_find(Graph *graph, char *key);


//--------/ Graph Algos /------------------------------------------------------
int    graph_dfs(Graph *graph, char *key_1, char *key_2); // Depth-First Search
Node*  graph_dfs_visit(Node *u, char *key_2, int *colors, Node **prevs);

int    graph_bf(Graph *graph, char *key_1, char *key_2); // The Bellman–Ford algorithm
Graph* graph_rn(Graph *graph, char *key_1, char *key_2); // Residual network


void   graph_numerate(Graph *graph);
void   graph_print_path(Node *node, Node **prevs);
// int edge_capacity();
// int edge_flow();
//auxiliary functions


//--------/ Graph Utils /------------------------------------------------------
void   graph_print(Graph *graph);
void   graph_make_graphviz(Graph *graph);

Graph* graph_load();
int    graph_save(Graph *graph);

int    graph_random(Graph *graph, int size);

#endif
