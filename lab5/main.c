#include "graph.h"
#include "menu.h"

int main(int argc, const char** argv) {

	Graph *graph = graph_new();

	// test
	graph_insert_node(graph, "1", 11, 11);
	graph_insert_node(graph, "2", 22, 22);
	graph_insert_node(graph, "3", 33, 33);
	graph_insert_node(graph, "4", 44, 44);
	graph_insert_node(graph, "5", 55, 55);
	graph_insert_node(graph, "6", 66, 66);

	graph_insert_edge(graph, "1", "2", 12);
	graph_insert_edge(graph, "2", "3", 23);
	graph_insert_edge(graph, "1", "3", 13);
	graph_insert_edge(graph, "6", "2", 62);
	graph_insert_edge(graph, "5", "4", 54);
	graph_insert_edge(graph, "3", "6", 36);
	graph_insert_edge(graph, "1", "4", 12);
	graph_insert_edge(graph, "2", "1", 23);
	graph_insert_edge(graph, "1", "6", 13);
	graph_insert_edge(graph, "6", "4", 62);
	graph_insert_edge(graph, "5", "2", 54);
	graph_insert_edge(graph, "3", "1", 36);

	start(graph);

	graph_delete(graph);

	return 0;
}
