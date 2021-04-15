#ifndef	__MENU_H
#define __MENU_H

int get_int(int *opt);
char* get_str();

int dialog_add(Tree *tree);
int dialog_find(Tree *tree);
int dialog_find_by_max_key(Tree *tree);
int dialog_delete(Tree *tree);
int dialog_show(Tree *tree);
int dialog_load(Tree *tree);

int dialog(const char *menu[], const int menu_size);

void start(Tree *tree);

#endif