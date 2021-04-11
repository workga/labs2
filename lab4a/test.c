#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <stdio.h>

#include "tree.h"
#include "test.h"

char* random_str(int len)
{
    char *str = (char*)calloc(len + 1, sizeof(char));
    if (!str) return NULL;

    char *cur = str;
    int m = 26; // 'z' + 1 - 'a' //122 + 1 - 97 = 26
    int d = 'a'; //97
    for (int i = 0; i < len; i++) {
        int r = rand()%m;
        *cur = d + r;
        cur++;
    }
    *cur = '\0';

    return str;
}

int test(int num) {
    Tree *tree = tree_new();


    srand(time(NULL));

    char **keys = (char**)calloc(num, sizeof(char*));
    if(!keys) return 1;

    double t_insert = 0;
    double t_find = 0;
    double t_remove = 0;


    int len = 10*(int)(log(num*num)/log(26.));

    int count = 0;
    while (count < num) {
        char *key = random_str(len);
        char *info = random_str(len);
        if (!key || !info) {
            free(key);
            free(info);
            
            for (int i = 0; i < num; i++) {
                free(keys[i]);
            }
            free(keys);
            tree_delete(tree);

            printf("[ERROR] Memory is over\n");
            return 1;
        }


        if (tree_find(tree, key)) {
            free(key);
            free(info);
            continue;
        }

        keys[count] = key;
        count++;

        clock_t t = clock();
        tree_insert(tree, key, info);
        t_insert += (double)(clock() - t)/CLOCKS_PER_SEC;

        free(info);
    }
    t_insert /= num;

    for (int i = 0; i < num; i++) {
        clock_t t = clock();
        tree_find(tree, keys[i]);
        t_find += (double)(clock() - t)/CLOCKS_PER_SEC;
    }
    t_find /= num;

    for (int i = 0; i < num; i++) {
        clock_t t = clock();
        tree_remove(tree, keys[i]);
        t_remove += (double)(clock() - t)/CLOCKS_PER_SEC;
    }
    t_remove /= num;

    for (int i = 0; i < num; i++) {
        free(keys[i]);
    }
    free(keys);

    printf("Number:\t%d\nLength:\t%d\n", num, len);
    printf("Type:\tinsert\nAverage time:\t%.10f\n", t_insert);
    printf("Type:\tfind\nAverage time:\t%.10f\n", t_find);
    printf("Type:\tremove\nAverage time:\t%.10f\n", t_remove);

    tree_delete(tree);

    return 0;
}