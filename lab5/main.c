#include "graph.h"
#include "menu.h"

const int DEBUG = 1;

int main(int argc, const char** argv) {

	Graph *graph = graph_load();
	if (!graph) {
        graph = graph_new();

		// test 3
		graph_insert_node(graph, "s", 6, 3);
		graph_insert_node(graph, "1", 6, 3);
		graph_insert_node(graph, "2", 6, 3);
		graph_insert_node(graph, "3", 6, 3);
		graph_insert_node(graph, "4", 6, 3);
		graph_insert_node(graph, "t", 6, 3);

		graph_insert_edge(graph, "s", "1", 1, 16);
	    graph_insert_edge(graph, "s", "3", 2, 13);
		graph_insert_edge(graph, "1", "3", 3, 10);
		graph_insert_edge(graph, "3", "1", 4, 4);
		graph_insert_edge(graph, "1", "2", 5, 12);
		graph_insert_edge(graph, "2", "3", 6, 9);
		graph_insert_edge(graph, "3", "4", 7, 14);
		graph_insert_edge(graph, "4", "2", 8, 7);
		graph_insert_edge(graph, "2", "t", 9, 20);
		graph_insert_edge(graph, "4", "t", 10, 4);

		graph_remove_node(graph, "2");

	}

	start(graph);

	graph_save(graph);
	graph_delete(graph);

	return 0;
}

	// test 1
	// graph_insert_node(graph, "1", 11, 11);
	// graph_insert_node(graph, "2", 22, 22);
	// graph_insert_node(graph, "3", 33, 33);
	// graph_insert_node(graph, "4", 44, 44);
	// graph_insert_node(graph, "5", 55, 55);
	// graph_insert_node(graph, "6", 66, 66);

	// graph_insert_edge(graph, "1", "2", 12);
	// graph_insert_edge(graph, "2", "3", 23);
	// graph_insert_edge(graph, "1", "3", 13);
	// graph_insert_edge(graph, "6", "2", 62);
	// graph_insert_edge(graph, "5", "4", 54);
	// graph_insert_edge(graph, "3", "6", 36);
	// graph_insert_edge(graph, "1", "4", 12);
	// graph_insert_edge(graph, "2", "1", 23);
	// graph_insert_edge(graph, "1", "6", 13);
	// graph_insert_edge(graph, "6", "4", 62);
	// graph_insert_edge(graph, "5", "2", 54);
	// graph_insert_edge(graph, "3", "1", 36);

	// test 2
	// graph_insert_node(graph, "1", 11, 11);
	// graph_insert_node(graph, "2", 22, 22);
	// graph_insert_node(graph, "3", 33, 33);
	// graph_insert_node(graph, "4", 44, 44);
	// graph_insert_node(graph, "5", 55, 55);
	// graph_insert_node(graph, "6", 66, 66);

	// graph_insert_edge(graph, "1", "2", 1, 1);
	// graph_insert_edge(graph, "2", "3", -2, 1);
	// graph_insert_edge(graph, "3", "4", 3, 1);
	// graph_insert_edge(graph, "4", "2", -2, 1);
	// //graph_insert_edge(graph, "4", "6", 1, 1);
	// graph_insert_edge(graph, "1", "5", 1, 1);
	// graph_insert_edge(graph, "5", "6", 1, 1);
