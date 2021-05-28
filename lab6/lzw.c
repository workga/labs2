СТРОКА = очередной символ из входного потока
WHILE входной поток не пуст DO
  СИМВОЛ = очередной символ из входного потока
  IF СТРОКА+СИМВОЛ в таблице строк THEN
    СТРОКА = СТРОКА+СИМВОЛ
  ELSE 
    вывести в выходной поток код для СТРОКА
    добавить в таблицу строк СТРОКА+СИМВОЛ
    СТРОКА = СИМВОЛ
  END of IF
END of WHILE
вывести в выходной поток код для СТРОКА


void lzw_encode(FILE *f_input, FILE *f_output) {
	Dict *dict = dict_new(INIT_CAPACITY);
	dict_init(dict);




	char cur_character = fgetc(f_input);

	unsigned long cur_prefix_index = dict->capacity
	unsigned long cur_index = dict_find(dict, cur_prefix_index, cur_character);
	Item cur_item = dict_get(dict, cur_index);

   


   

	dict_delete(dict);
}


