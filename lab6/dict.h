#ifndef __DICT_H
#define __DICT_H

// #define INIT_CAPACITY 512
#define INIT_CAPACITY 512
#define MAX_FULLNESS  0.75
#define EXTEND_RATIO  2

const char DEBUG;

enum Error {
	OK,
	ERROR,
	OUT_OF_MEMORY,
	COULD_NOT_READ_FILE,
	DUPLICATE_ITEM
};

typedef struct Item {
   char busy;
   char character;
   unsigned long prefix_index;
   unsigned long new_prefix_index;
   short padding1;
   short padding2;
   short padding3;
} Item;

typedef struct Dict {
	unsigned long size;
	unsigned long capacity;
	FILE *fp;
} Dict;


//----------/ CREATE, INIT, DELETE /-----------------------------------------------------------

// creates new dict
Dict* dict_new(unsigned long capacity);

// fill dict with 256 single characters;
void  dict_init(Dict *dict);

// delete dict and its file
void  dict_delete(Dict *dict);



//----------/ INTERFACE /------------------------------------------------------

// return free index for insertion or capacity, if item is found
unsigned long dict_find(Dict *dict, unsigned long prefix_index, char character);

// insert item (includes existence check and extension)
unsigned long dict_insert(Dict *dict, unsigned long prefix_index, char character);

// create new file, copy all items to it
unsigned long dict_extend(Dict *dict);

// print all items (including not busy) and queue
void dict_print(Dict *dict);



//----------/ FILE /-----------------------------------------------------------

// delete corresponding file
void dict_delete_file(Dict *dict);

// read item from file by index
Item dict_get(Dict *dict, unsigned long index);

// write item to file by index
void dict_set(Dict *dict, unsigned long index, char busy, 
	          unsigned long prefix_index, char character, unsigned long new_prefix_index);

// read index from queue by q_index
unsigned long dict_get_queue(Dict *dict, unsigned long q_index);

// write index to the end of file, i.e. add it to queue
void dict_set_queue(Dict *dict, unsigned long index);



//----------/ AUXILIARY /------------------------------------------------------

// create filename for dict using its capacity
char* dict_generate_name(unsigned long capacity);

// calculate next capacity value
unsigned long dict_next_capacity(Dict *dict);

// hash
unsigned long dict_hash(Dict *dict, unsigned long prefix_index, char character);

// get next probe of index
unsigned long dict_iterate(Dict *dict, unsigned long index);



#endif
