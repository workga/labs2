#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "graph.h"
#include "menu.h"


//----/  Input /---------------------------------------------------------------
int get_int(int *opt) {
	int error;
	do {
		error = scanf("%d", opt);
		if (error < 0) return END_OF_FILE;//EOF
		if (error == 0) {
			printf("Invalid input. Please, repeate.\n");
			scanf("%*[^\n]");
		}
	} while (error == 0);
	scanf("%*c");
	return 0;
}


int get_float(float *opt) {
	int error;
	do {
		error = scanf("%f", opt);
		if (error < 0) return END_OF_FILE;//EOF
		if (error == 0) {
			printf("Invalid input. Please, repeate.\n");
			scanf("%*[^\n]");
		}
	} while (error == 0);
	scanf("%*c");
	return 0;
}


int get_double(double *opt) {
	int error;
	do {
		error = scanf("%lf", opt);
		if (error < 0) return END_OF_FILE;//EOF
		if (error == 0) {
			printf("Invalid input. Please, repeate.\n");
			scanf("%*[^\n]");
		}
	} while (error == 0);
	scanf("%*c");
	return 0;
}


char* get_str() {
    char buf[81];
    char *res = NULL;
    int len = 0;
    int n = 0;

    do {
        n = scanf("%80[^\n]", buf);
        if (n < 0) {
        	free(res);
        	return NULL;
        } else if (n > 0) {
            int new_len = len + strlen(buf);
            res = realloc(res, new_len + 1);
            memcpy(res + len, buf, strlen(buf));
            len = new_len;
        } else {
            scanf("%*c");
        }
    } while (n > 0);

    if (len > 0) {
    	res[len] = '\0';
    } else {
    	res = (char*)calloc(1, sizeof(char));
    }

    return res;
}


//----/  Dialog /--------------------------------------------------------------
int dialog_add_node(Graph *graph) {
	printf("Enter key (string):\n");
	char *key;
	if(!(key = get_str())) return END_OF_FILE;

	printf("Enter x (double):\n");
	double x;
	if(get_double(&x)) {
		free(key);
		return OUT_OF_MEMORY;
	}

	printf("Enter y (double):\n");
	double y;
	if(get_double(&y)) {
		free(key);
		return OUT_OF_MEMORY;
	}

	int e = graph_insert_node(graph, key, x, y);

	free(key);
	return e;
}


int dialog_add_edge(Graph *graph) {
	printf("Enter key_1 (string):\n");
	char *key_1;
	if(!(key_1 = get_str())) return END_OF_FILE;

	printf("Enter key_2 (string):\n");
	char *key_2;
	if(!(key_2 = get_str())) {
		free(key_2);
		return END_OF_FILE;
	}

	printf("Enter weight (int):\n");
	int w;
	if(get_int(&w)) {
		free(key_1);
		free(key_2);
		return OUT_OF_MEMORY;
	}

	printf("Enter capacity (int, > 0):\n");
	int c;
	if(get_int(&c)) {
		free(key_1);
		free(key_2);
		return OUT_OF_MEMORY;
	}
	if (c <= 0) {
		free(key_1);
		free(key_2);
		return INVALID_INPUT;
	}


	int e = graph_insert_edge(graph, key_1, key_2, w, c);

	free(key_1);
	free(key_2);
	return e;
}


int dialog_delete_node(Graph *graph) {
	printf("Enter key (string):\n");
	char *key;
	if(!(key = get_str())) return END_OF_FILE;

	int e = graph_remove_node(graph, key);

	free(key);
	return e;
}


int dialog_delete_edge(Graph *graph) {
	printf("Enter key_1 (string):\n");
	char *key_1;
	if(!(key_1 = get_str())) return END_OF_FILE;

	printf("Enter key_2 (string):\n");
	char *key_2;
	if(!(key_2 = get_str())) {
		free(key_2);
		return END_OF_FILE;
	}


	int e = graph_remove_edge(graph, key_1, key_2);

	free(key_1);
	free(key_2);
	return e;
}


int dialog_dfs(Graph *graph) {
	printf("Enter key_1 (string):\n");
	char *key_1;
	if(!(key_1 = get_str())) return END_OF_FILE;

	printf("Enter key_2 (string):\n");
	char *key_2;
	if(!(key_2 = get_str())) {
		free(key_2);
		return END_OF_FILE;
	}

	int e = graph_dfs(graph, key_1, key_2);

	free(key_1);
	free(key_2);
	return e;
}


int dialog_bf(Graph *graph) {
	printf("Enter key_1 (string):\n");
	char *key_1;
	if(!(key_1 = get_str())) return END_OF_FILE;

	printf("Enter key_2 (string):\n");
	char *key_2;
	if(!(key_2 = get_str())) {
		free(key_2);
		return END_OF_FILE;
	}

	int e = graph_bf(graph, key_1, key_2);

	free(key_1);
	free(key_2);
	return e;
}


int dialog_rn(Graph *graph) {
	printf("Enter key_1 (string):\n");
	char *key_1;
	if(!(key_1 = get_str())) return END_OF_FILE;

	printf("Enter key_2 (string):\n");
	char *key_2;
	if(!(key_2 = get_str())) {
		free(key_2);
		return END_OF_FILE;
	}

	int e = graph_rn_max_flow(graph, key_1, key_2);

	free(key_1);
	free(key_2);
	return e;
}


int dialog_show(Graph *graph) {
	printf("Choose mode (int):\n1) Adjacency lists\n2) Graphviz\n");
	int opt;
	if(get_int(&opt)) return OUT_OF_MEMORY;

	if (opt == 1) graph_print(graph);
	else if (opt == 2) graph_make_graphviz(graph);
	else return INVALID_INPUT;

	return OK;
}


int dialog_random(Graph *graph) {
	printf("Enter number of nodes: \n");
	int size;
	if(get_int(&size)) return OUT_OF_MEMORY;


	int e = graph_random(graph, size);
	return e;
}


int dialog(const char *menu[], const int menu_size) {
	char *error_msg = "";
	int opt;

	do{
		printf("%s\n", error_msg);
		error_msg = "Invalid input. Please, repeate.";

		for (int i = 0; i < menu_size; i++) {
			printf("%s\n", menu[i]);
		}
		printf("%s", "Make your choice: ");

		if(get_int(&opt)) {
			opt = 0;
		}
	} while (opt < 0 || opt >= menu_size);

	return opt;
}


void print_error(int e) {
	switch (e) {
		case OK:
			break;
		case OUT_OF_MEMORY:
			printf("[ERROR] Out of memory.\n");
			break;
		case DUPLICATE_KEY:
			printf("[ERROR] Duplicate key.\n");
			break;
		case DUPLICATE_EDGE:
			printf("[ERROR] Duplicate edge.\n");
			break;
		case KEY_NOT_FOUND:
			printf("[ERROR] Key not found.\n");
			break;
		case EDGE_NOT_FOUND:
			printf("[ERROR] Edge not found.\n");
			break;
		case INVALID_INPUT:
			printf("[ERROR] Invalid input.\n");
			break;
		case END_OF_FILE:
			printf("[ERROR] EOF found.\n");
			break;
		case PATH_NOT_FOUND:
			printf("[ERROR] Path not found.\n");
			break;
		case NEGATIVE_LOOP_FOUND:
			printf("[ERROR] Nagative loop found.\n");
			break;
		default:
			break;
	}
}


void start(Graph *graph) {
	const char *menu[] = {"0. Quit\n",
					  	  "1. Add node",
					      "2. Add edge",
					      "3. Delete node",
					      "4. Delete edge\n",
					      "5. DFS",
					      "6. Bellman-Ford",
					      "7. Residual network\n",
					      "8. Show",
					  	  "9. Random"};
	const int menu_size = sizeof(menu)/sizeof(menu[0]);

	int (*dialog_functions[])(Graph*) = {NULL,
										dialog_add_node,
										dialog_add_edge,
										dialog_delete_node,
										dialog_delete_edge,
										dialog_dfs,
										dialog_bf,
										dialog_rn,
										dialog_show,
										dialog_random};

	int opt;
	while((opt = dialog(menu, menu_size))) {
		if (opt == 0) break;
		int e = dialog_functions[opt](graph);
		print_error(e);
		if (e == OUT_OF_MEMORY || e == END_OF_FILE) break;
	}

	printf("\nProgram finished.\n");
}
