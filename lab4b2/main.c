#include <stdlib.h>
#include <time.h>
#include <stdio.h>

#include "tree.h"

const int DEBUG = 1;
const int MAX_UINT = 100;

int get_int(int *opt);
int get_uint(unsigned int *opt);


int main(int argc, const char** argv) {
	FILE *f_bin;
	unsigned int key;

	printf("Enter number of random uints (or 0 if you want to use existing file)\n");
	int opt;
	if(get_int(&opt)) {
		printf("Finished");
		return 0;
	}

	if (opt > 0) {
		f_bin = fopen("input.bin", "wb");
		if (!f_bin) {
			printf("Couldn't read the file (wb).\n");
			return 0;
		}

		srand(time(NULL));   
		while (opt > 0) {
			key = (unsigned int)rand() % 101;
			fwrite(&key, sizeof(unsigned int), 1, f_bin);

			opt--;
		}

		fclose(f_bin);
	}

	f_bin = fopen("input.bin", "rb");
	if (!f_bin) {
		printf("Couldn't read the file (rb).\n");
		return 0;
	}


	Node *tree = NULL;

	long int pos;
	pos = ftell(f_bin);
	while(fread(&key, sizeof(unsigned int), 1, f_bin)) {
		tree_insert(&tree, key, pos);
		pos = ftell(f_bin);
	}

	tree_draw(tree, 0);

	printf("Enter uint to find\n");
	while(!get_uint(&key)) {
		Info *info = tree_find_info(tree, key, 1);
		if (info) printf("Found pos: %ld\n", info->pos);
		else printf("Not found\n");
	}

	tree_delete(tree);
	return 0;
}



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


int get_uint(unsigned int *opt) {
	int error;
	do {
		error = scanf("%i", opt);
		if (error < 0) return 1;//EOF
		if (error == 0) {
			printf("Invalid input. Please, repeate.\n");
			scanf("%*[^\n]");
		}
	} while (error == 0);
	scanf("%*c");
	return 0;
}