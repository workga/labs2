#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "tree.h"
#include "menu.h"
#include "test.h"


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

//-----------------------------------------------------------------------------


int dialog_add(Tree *tree) {
	printf("[ADD]\n");

	printf("Enter key:\n");
	char *key;
	if(!(key = get_str())) return 1;

	printf("Enter info:\n");
	char *info;
	if(!(info = get_str())) return 1;


	tree_insert(tree, key, info);


	free(key);
	free(info);
	return 0;
}


int dialog_find(Tree *tree) {
	printf("[FIND]\n");

	printf("Enter key:\n");
	char *key;
	if(!(key = get_str())) return 1;


	const Node *ptr = tree_find(tree, key);
	if (!ptr)
        printf("[FIND] Key \"%s\" not found\n", key);
	else
        printf("[FIND] Found: Key: \"%s\"\tInfo: \"%s\"\n", ptr->key, ptr->info);

	free(key);
	return 0;
}


int dialog_find_by_max_key(Tree *tree) {
	printf("[MAX KEY]\n");


	const Node *ptr = tree_find_by_max_key(tree);
	if (!ptr) printf("[MAX KEY] Tree is empty\n");
    else printf("[MAX KEY] Found: Key: \"%s\"\tInfo: \"%s\"\n", ptr->key, ptr->info);

	return 0;
}


int dialog_delete(Tree *tree) {
	printf("[DELETE]\n");

	printf("Enter key:\n");
	char *key;
	if(!(key = get_str())) return 1;


	tree_remove(tree, key);


	free(key);
	return 0;
}


int dialog_show(Tree *tree) {
	printf("[SHOW]\n");

	printf("Choose:\n1) order by key\n2) print as a tree\n3) graphviz (DOT)\n");
	int opt;
	if(get_int(&opt)) return 1;

	if (opt == 1) {
		tree_print(tree);
	} else if (opt == 2) {
		tree_draw(tree);
	} else if (opt == 3) {
		tree_make_graphviz(tree);
	} else {
		printf("[ERROR] Invalid option!");
	}

	return 0;
}

int dialog_test(Tree *tree) {
	printf("[Test]\n");

	printf("Enter the number of elements\n");
	int num;
	if(get_int(&num)) return 1;

	if (test(num)) return 1;

	return 0;
}

int dialog_load(Tree *tree) {
	if(tree_load(tree)) printf("Couldn't load the file");
	else printf("File was loaded");
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


void start(Tree *tree) {
	const char *menu[] = {"0. Quit",
					  	  "1. Add",
					      "2. Find",
					      "3. Find by max key",
					      "4. Delete",
					      "5. Show",
					  	  "6. Test",
					  	  "7. Load"};
	const int menu_size = sizeof(menu)/sizeof(menu[0]);

	int (*dialog_functions[])(Tree*) = {NULL,
										dialog_add,
										dialog_find,
										dialog_find_by_max_key,
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
