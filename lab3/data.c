#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>
#include <string.h>

#include "data.h"

Data* data_new(Info info) {
	Data *data = (Data*)calloc(1, sizeof(Data));
	Info *new_info = (Info*)calloc(1, sizeof(Info));
	char *str = (char*)calloc(strlen(info.str) + 1, sizeof(char));

	if (!data || !new_info || !str) {
		free(str);
		free(new_info);
		free(data);
		return NULL;
	}

	strcpy(str, info.str);

	new_info->int_1 = info.int_1;
	new_info->int_2 = info.int_2;
	new_info->str = str;
	data->info = new_info;
	data->key_2 = NULL;

	return data;
}

void data_delete(Data *data) {
	free(data->info->str);
	free(data->info);
	free(data->key_2);
	free(data);
}


void data_print(Data *data) {
	if (!data) printf("(NULL)");
	else printf("(%d, %d, %s)",
				data->info->int_1,
				data->info->int_2,
				data->info->str);
}