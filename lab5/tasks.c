#include <stdlib.h>
#include <stdio.h>
#include <float.h>

#include "graph.h"
#include "tasks.h"

int find_path(Graph *graph, double x1, double y1, double x2, double y2) {
	Node *node_1 = find_nearest_node(graph, x1, y1);
	Node *node_2 = find_nearest_node(graph, x2, y2);

	if(node_1 == node_2) {
		printf("These points are too close.\n");
		return OK;
	}

	printf("Nearest node for (x1, y1): %s\n", node_1->key);
	printf("Nearest node for (x2, y2): %s\n", node_2->key);

	int e = graph_bf(graph, node_1->key, node_2->key, 1);
	return e;
}


Node* find_nearest_node(Graph *graph, double x, double y) {
	Node *cur_node = graph->list;
	Node *found_node = cur_node;
	double cur_node_sq_dist = DBL_MAX;

	while (cur_node) {
		double new_sq_dist = calculate_sq_dist(x, y, cur_node->x, cur_node->y);
		if (new_sq_dist < cur_node_sq_dist) {
			cur_node_sq_dist = new_sq_dist;
			found_node = cur_node;
		}
		cur_node = cur_node->next;
	}

	return found_node;
}


double calculate_sq_dist(double x1, double y1, double x2, double y2) {
	return (x2 - x1)*(x2 - x1) + (y2 - y1)*(y2 - y1);
}


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


char* int_to_str(int n) {

	int len = 0;

	if (n == 0) len = 1;
	else {
		int i = n;
		while (i > 0) {
			i /= 10;
			len++;
		}
	}

	char *str = (char*)calloc(len + 1, sizeof(char));
	sprintf(str, "%d", n);
	str[len] = '\0';
	//itoa(n, str, 10);
	return str;
}

// char* int_to_str(int n) {
// 	int len = 0;

// 	int i = n;
// 	while (i > 0) {
// 		i /= 10;
// 		len++;
// 	}

// 	char *str = (char*)calloc(len + 1, sizeof(char));
// 	sprintf(str, "%d", n);
// 	return str;
// }