#ifndef __DATA_H
#define __DATA_H

typedef struct Info {
	int int_1;
	int int_2;
	char *str;
} Info;


typedef struct Data {
	Info *info;
	unsigned int key_1;
	char *key_2;
	int release;
} Data;

typedef struct Data_array {
	Data **array;
	int len;
} Data_array;

Data* data_new(Info info);
void data_delete(Data *data);

void data_print(const Data *data);
void info_print(const Info *info);

#endif