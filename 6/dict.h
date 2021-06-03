#ifndef __DICT_H
#define __DICT_H


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

typedef unsigned int uint;

typedef struct Item {
   char busy;
   char character;
   uint prefix_index;
   uint new_prefix_index;
   uint padding;
} Item;

typedef struct Dict {
	uint size;
	uint capacity;
	FILE *fp;
} Dict;


//----------/ CREATE, INIT, DELETE /-----------------------------------------------------------

// creates new dict
Dict* dict_new(uint capacity);

// fill dict with 256 single characters;
void  dict_init(Dict *dict);

// delete dict and its file
void  dict_delete(Dict *dict);



//----------/ INTERFACE /------------------------------------------------------

// return free index for insertion or capacity, if item is found
uint dict_find(Dict *dict, uint prefix_index, char character);

// insert item (includes existence check and extension)
uint dict_insert(Dict *dict, uint prefix_index, char character);

// create new file, copy all items to it
uint dict_extend(Dict *dict);

// print all items (including not busy) and queue
void dict_print(Dict *dict);



//----------/ FILE /-----------------------------------------------------------

// delete corresponding file
void dict_delete_file(Dict *dict);

// read item from file by index
Item dict_get(Dict *dict, uint index);

// write item to file by index
void dict_set(Dict *dict, uint index, char busy, 
	          uint prefix_index, char character, uint new_prefix_index);

// read index from queue by q_index
uint dict_get_queue(Dict *dict, uint q_index);

// write index to the end of file, i.e. add it to queue
void dict_set_queue(Dict *dict, uint index);



//----------/ AUXILIARY /------------------------------------------------------

// create filename for dict using its capacity
char* dict_generate_name(uint capacity);

// calculate next capacity value
uint dict_next_capacity(Dict *dict);

// hash
uint dict_hash(Dict *dict, uint prefix_index, char character);

// get next probe of index
uint dict_iterate(Dict *dict, uint index);



#endif
