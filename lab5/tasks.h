#ifndef __TASKS_H
#define __TASKS_H

int     find_path(Graph *graph, double x1, double y1, double x2, double y2);
Node*	find_nearest_node(Graph *graph, double x, double y);
double 	calculate_sq_dist(double x1, double y1, double x2, double y2);

int 	load_map(Graph *graph);
char* 	int_to_str(int n);

#endif