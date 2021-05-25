#include <stdlib.h>
#include <stdio.h>

#include "graph.h"
#include "tasks.h"

int load_map(Graph *graph) {
	graph_remove_all(graph);

	// nodes
	int e;
	FILE *fp = fopen("map_nodes.txt", "r");
	if (!fp) {
		graph_remove_all(graph);
		return COULD_NOT_READ_FILE;
	}

	int id;
	double x, y;
	while ( (e = fscanf(fp, "%d%lf%lf\n", &id, &x, &y)) > 0 ) {
		char *key = int_to_str(id);
		graph_insert_node(graph, key, x, y);
		free(key);
	}

	fclose(fp);



	// edges
	fp = fopen("map_edges.txt", "r");
	if (!fp) {
		graph_remove_all(graph);
		return COULD_NOT_READ_FILE;
	}

	int id1, id2;
	double dw;
	int w;
	while ( (e = fscanf(fp, "%d%d%d%lf\n", &id, &id1, &id2, &dw)) > 0 ) {
		char *key_1 = int_to_str(id1);
		char *key_2 = int_to_str(id2);

		w = (int)(1000*dw);
		graph_insert_edge(graph, key_1, key_2, w, 0);
		graph_insert_edge(graph, key_2, key_1, w, 0);

		free(key_1);
		free(key_2);
	}

	fclose(fp);

	return OK;
}

// char* int_to_str(int n) {

// 	int len = 0;

// 	if (n == 0) len = 1;
// 	else {
// 		int i = n;
// 		while (i > 0) {
// 			i /= 10;
// 			len++;
// 		}
// 	}

// 	char *str = (char*)calloc(len + 1, sizeof(char));
// 	sprintf(str, "%d", n);
// 	str[len] = '\0';
// 	//itoa(n, str, 10);
// 	return str;
// }

char* int_to_str(int n) {
	int len = 0;

	int i = n;
	while (i > 0) {
		i /= 10;
		len++;
	}

	char *str = (char*)calloc(len + 1, sizeof(char));
	sprintf(str, "%d", n);
	return str;
}