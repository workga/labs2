#include <stdlib.h>
#include <stdio.h>

#include "dict.h"
#include "lzw.h"

// СТРОКА = очередной символ из входного потока
// WHILE входной поток не пуст DO
//   СИМВОЛ = очередной символ из входного потока
//   IF СТРОКА+СИМВОЛ в таблице строк THEN
//     СТРОКА = СТРОКА+СИМВОЛ
//   ELSE
//     вывести в выходной поток код для СТРОКА
//     добавить в таблицу строк СТРОКА+СИМВОЛ
//     СТРОКА = СИМВОЛ
//   END of IF
// END of WHILE
// вывести в выходной поток код для СТРОКА


void lzw_encode(FILE *fp_in, FILE *fp_out) {
	Dict *dict = dict_new(INIT_CAPACITY);
	dict_init(dict);

	uint prefix_index = dict->capacity;
	int character;
	while ((character = fgetc(fp_in)) != EOF) {
		uint i = dict_find(dict, prefix_index, character);
		if (dict_get(dict, i).busy == 1) { 	// found
			prefix_index = i;
		} else { // not found
			fwrite(&prefix_index, sizeof(uint), 1, fp_out);
			//printf("%u\n", prefix_index);
			dict_insert(dict, prefix_index, character);
			// dict_print(dict);
			prefix_index = dict_find(dict, dict->capacity, character);
		}
	}

	fwrite(&prefix_index, sizeof(uint), 1, fp_out);
	//printf("%u\n", prefix_index);

	// uint index;
	// fseek(fp_out, 0, SEEK_SET);
	// while (fread(&index, sizeof(uint), 1, fp_out) > 0) {
	// 	printf("%u\n", index);
	// }

	// dict_print(dict);

	// for (uint i = 0; i < dict->capacity; i++) {
	// 	write_word(dict, i, stdout);
	// }

	dict_delete(dict);
}


// читать СТАРЫЙ_КОД
// вывести СТАРЫЙ_КОД
// WHILE входной поток не пуст DO
//   читать НОВЫЙ_КОД
//   СТРОКА = перевести НОВЫЙ_КОД
//   вывести СТРОКУ
//   СИМВОЛ = первый символ СТРОКИ
//   добавить в таблицу перевода СТАРЫЙ_КОД+СИМВОЛ
//   СТАРЫЙ_КОД = НОВЫЙ_КОД
// END of WHILE


// читать СТАРЫЙ_КОД
// вывести СТАРЫЙ_КОД
// СИМВОЛ = СТАРЫЙ_КОД
// WHILE входной поток не пуст DO
//   читать НОВЫЙ_КОД
//   IF NOT в таблице перевода НОВЫЙ_КОД THEN
//     СТРОКА = перевести СТАРЫЙ_КОД
//     СТРОКА = СТРОКА+СИМВОЛ
//   ELSE
//     СТРОКА = перевести НОВЫЙ_КОД
//   END of IF
//   вывести СТРОКУ
//   СИМВОЛ = первый символ СТРОКИ
//   добавить в таблицу перевода СТАРЫЙ_КОД+СИМВОЛ
//   СТАРЫЙ_КОД = НОВЫЙ_КОД
// END of WHILE


void lzw_decode(FILE *fp_in, FILE *fp_out) {
	fseek(fp_in, 0, SEEK_SET);
	Dict *dict = dict_new(INIT_CAPACITY);
	dict_init(dict);

	Item old_item;
	uint old_index;
	char character;

	uint word;

	fread(&old_index, sizeof(uint), 1, fp_in);
	write_word(dict, old_index, fp_out);
	old_item = dict_get(dict, old_index);
	character = old_item.character;

	uint new_index;
	while (fread(&new_index, sizeof(uint), 1, fp_in) > 0) {
		if ((old_item = dict_get(dict, new_index)).busy == 0) { // not found
			word = dict_find(dict, old_index, character);
		} else { // found
			word = new_index;
		}
		write_word(dict, word, fp_out);
		character = dict_get(dict, word).character;
		dict_insert(dict, old_index, character);
		old_index = new_index;
	}


	//dict_print(dict);
	dict_delete(dict);
}






// write word to file (file must be opened and position must be set)
void write_word(Dict *dict, uint index, FILE *fp) {
	if (index == dict->capacity) return;

   	Item item = dict_get(dict, index);
   	if (item.busy == 0) return;
	//printf("[%u]\t\"", index);
   	do {
       	fputc(item.character, fp);
       	//printf("%c", item.character);
   		item = dict_get(dict, item.prefix_index);
   	} while (item.prefix_index != dict->capacity);
	fputc(item.character, fp);
	//printf("%c", item.character);
	//printf("\"\n");
	fputc('.', fp);
}
