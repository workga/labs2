#ifndef	__MENU_H
#define __MENU_H


//----/  Input /---------------------------------------------------------------
int get_int(int *opt);
int get_float(float *opt);
char* get_str();


//----/  Dialog /--------------------------------------------------------------
int dialog_add(Node **tree);
int dialog_find(Node **tree);
int dialog_find_min_greater(Node **tree);
int dialog_delete(Node **tree);
int dialog_show(Node **tree);
int dialog_load(Node **tree);
int dialog_random(Node **tree);

int dialog(const char *menu[], const int menu_size);

void start(Node **tree);

#endif