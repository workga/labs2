#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "tree.h"
#include "menu.h"


//----/  Input /---------------------------------------------------------------
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


int get_float(float *opt) {
	int error;
	do {
		error = scanf("%f", opt);
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
int dialog_add(Node **tree) {
	printf("[ADD]\n");

	printf("Enter key (int):\n");
	int key;
	if(get_int(&key)) return 1;

	printf("Enter info.flt (float):\n");
	float flt;
	if(get_float(&flt)) return 1;

	printf("Enter info str (string):\n");
	char *str;
	if(!(str = get_str())) return 1;

	//-------------------------------------------


	if (tree_insert(tree, key, flt, str)) return 1;

	free(str);
	return 0;
}


int dialog_find(Node **tree) {
	printf("[FIND]\n");

	printf("Enter key (int):\n");
	int key;
	if(get_int(&key)) return 1;

	printf("Enter release (int):\n");
	int num;
	if(get_int(&num)) return 1;

	//-------------------------------------------


	Info *info  = tree_find_info(*tree, key, num);
	if (!info) printf("Not found");
	else printf("Found: [%4.2f, \'%s\'])",
				info->flt, info->str);

	return 0;
}


int dialog_find_min_greater(Node **tree) {
	printf("[MIN GREATER]\n");

	printf("Enter key (int):\n");
	int key;
	if(get_int(&key)) return 1;

	//-------------------------------------------


	Info *info = tree_find_min_greater(*tree, key);
	if (!info) printf("There is no greater elements");
	else printf("Next element: [%4.2f, \'%s\'])",
				info->flt, info->str);
	return 0;
}


int dialog_delete(Node **tree) {
	printf("[DELETE]\n");

	printf("Enter key (int):\n");
	int key;
	if(get_int(&key)) return 1;

	//-------------------------------------------


	if (tree_remove(tree, key)) {
		printf("Key not found");
	}

	return 0;
}


int dialog_show(Node **tree) {
	printf("[SHOW]\n");

	printf("Choose:\n1) order by key\n2) print as a tree\n3) graphviz (DOT)\n");
	int opt;
	if(get_int(&opt)) return 1;

	//-------------------------------------------


	if (opt == 1) {
		tree_print(*tree);
	} else if (opt == 2) {
		tree_draw(*tree, 0);
	} else if (opt == 3) {
		tree_make_graphviz(*tree, 1);
	} else {
		printf("[ERROR] Invalid option!");
	}

	return 0;
}

int dialog_test(Node **tree) {
	printf("[TEST]\n");
	printf("Not implemented yet\n");
	//...
	return 0;
}

int dialog_load(Node **tree) {
	printf("[LOAD]\n");
	printf("Not implemented yet\n");
	//...
	return 0;
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


void start(Node **tree) {
	const char *menu[] = {"0. Quit",
					  	  "1. Add",
					      "2. Find",
					      "3. Find min greater",
					      "4. Delete",
					      "5. Show",
					  	  "6. Test",
					  	  "7. Load"};
	const int menu_size = sizeof(menu)/sizeof(menu[0]);

	int (*dialog_functions[])(Node**) = {NULL,
										dialog_add,
										dialog_find,
										dialog_find_min_greater,
										dialog_delete,
										dialog_show,
										dialog_test,
										dialog_load};

	int opt;
	while((opt = dialog(menu, menu_size))) {
		if (opt == 0 || dialog_functions[opt](tree)) break;
	}

	printf("\nProgram finished.\n");
}
