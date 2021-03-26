#ifndef	__MENU_H
#define __MENU_H

int get_int(int *opt);
char* get_str();

int dialog_add(Table* table);
int dialog_find(Table* table);
int dialog_delete(Table* table);
int dialog_show(Table* table);

int dialog(const char *menu[], int menu_size);


void start(Table *table);

#endif