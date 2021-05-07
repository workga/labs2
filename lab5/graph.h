#ifndef __GRAPH_H
#define __GRAPH_H

enum Error {
	OK,
	OUT_OF_MEMORY,
	DUPLICATE_KEY,
	DUPLICATE_EDGE,
	KEY_NOT_FOUND,
	EDGE_NOT_FOUND,
	INVALID_INPUT,
	END_OF_FILE
};

//--------/ Structures /-------------------------------------------------------
struct Node;

typedef struct Edge {
	int w;
	struct Node *near;
	struct Edge *next;
} Edge;

typedef struct Node {
	char *key;
	double x, y;
	struct Edge *list;
	struct Node *next;
} Node;

typedef struct Graph { // maybe add size???
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



//--------/ Graph Algos /------------------------------------------------------
void   graph_dfs(Graph *graph, char *key_1, char *key_2); // Depth-First Search
void   graph_bf(Graph *graph, char *key_1, char *key_2); // The Bellman–Ford algorithm
Graph* graph_rn(Graph *graph, char *key_1, char *key_2); // Residual network

//auxiliary functions


//--------/ Graph Utils /------------------------------------------------------
void   graph_print(Graph *graph);
void   graph_make_graphviz(Graph *graph);

Graph* graph_load();
int    graph_save(Graph *graph);

int    graph_random(Graph *graph, int size);

#endif
