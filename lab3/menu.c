#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "data.h"
#include "table.h"
#include "menu.h"

int get_int(int *opt) {
	int error;
	do {
		error = scanf("%d", opt);
		if (error < 0) return 1;//EOF
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
            if (!res) {
                return NULL;
            }
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
    }
    else {
        res = calloc(1, sizeof(char));
    }

    return res;
}


int dialog_add(Table* table) {
	printf("[ADD]\n");

	printf("Enter keys.\n");

	int key_1;
	int parent_key_1;
	char *key_2;

	printf("key_1: ");
	if(get_int(&key_1)) return 1;


	printf("parent_key_1: ");
	if(get_int(&parent_key_1)) return 1;


	printf("key_2: ");
	if(!(key_2 = get_str())) return 1;



	printf("Enter info.\n");

	int int_1;
	int int_2;
	char *str;

	printf("int_1: ");
	if(get_int(&int_1)) {
		free(key_2);
		return 1;
	}

	printf("int_2: ");
	if(get_int(&int_2)) {
		free(key_2);
		return 1;
	}

	printf("str: ");
	if(!(str = get_str())) {
		free(key_2);
		return 1;
	}



	Info info;
	info.int_1 = int_1;
	info.int_2 = int_2;
	info.str   = str;

	table_insert(table, key_1, parent_key_1, key_2, info);

	free(key_2);
	free(str);
	return 0;
}


int dialog_find(Table* table) {
	printf("[FIND]\n");

	printf("Enter keys.\n");

	int key_1;
	char *key_2;

	printf("key_1: ");
	if(get_int(&key_1)) return 1;

	printf("key_2: ");
	if(!(key_2 = get_str())) return 1;

	const Info *info = table_find(table, key_1, key_2);
	info_print(info);
	printf("\n");

	return 0;
}


int dialog_delete(Table* table) {
	printf("[DELETE]\n");

	printf("Enter keys.\n");

	int key_1;
	char *key_2;
	int recurr;

	printf("key_1: ");
	if(get_int(&key_1)) return 1;

	printf("key_2: ");
	if(!(key_2 = get_str())) return 1;

	printf("Delete recurrently? (0 - no, else - yes): ");
	if(get_int(&recurr)) return 1;

	table_remove(table, key_1, key_2, recurr);

	free(key_2);

	return 0;
}


int dialog_show(Table* table) {
	int opt;
	printf("[SHOW]\n");

	printf("Choose 1st or 2nd mode: ");
	if(get_int(&opt)) return 1;

	table_print(table, opt);

	return 0;
}


int dialog(const char *menu[], int menu_size) {
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

void start(Table *table) {
	const char *menu[] = {"0. Quit",
					  "1. Add",
					  "2. Find",
					  "3. Delete",
					  "4. Show"};
	const int menu_size = sizeof(menu)/sizeof(menu[0]);

	int (*dialog_functions[])(Table*) = {NULL, dialog_add, dialog_find, dialog_delete, dialog_show};

	int opt;
	while(opt = dialog(menu, menu_size)) {
		if (opt == 0 || dialog_functions[opt](table)) break;
	}

	printf("\nProgram finished.\n");
}
