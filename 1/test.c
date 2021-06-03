#include <stdlib.h>
#include <stdio.h>
#include <time.h>

int main(int argc, const char** argv) {
	srand(time(NULL));

	int m = rand()%15 + 1;
	printf("%d\n", m);

	for(int i = 0; i < m; i++) {
		int n = rand()%15 + 1;
		printf("%d\n", n);

		for(int j = 0; j < n; j++) {
			printf("%d ", rand()%2001 - 1000);
		}

		printf("\n");
	}

	return 0;
}