#ifndef	__MENU_H
#define __MENU_H


//----/  Input /---------------------------------------------------------------
int   get_int(int *opt);
int   get_float(float *opt);
int   get_double(double *opt);
char* get_str();


//----/  Dialog /--------------------------------------------------------------
int dialog_add_node(Graph *graph);
int dialog_add_edge(Graph *graph);

int dialog_delete_node(Graph *graph);
int dialog_delete_edge(Graph *graph);

int dialog_dfs(Graph *graph);
int dialog_bf(Graph *graph);
int dialog_rn(Graph *graph);

int dialog_show(Graph *graph);
int dialog_random(Graph *graph);
int dialog_test(Graph *graph);

int dialog_load_map(Graph *graph);
int dialog_find_path(Graph *graph);

int  dialog(const char *menu[], const int menu_size);
void print_error(int e);

void start(Graph *graph);

#endif