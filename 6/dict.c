#include <stdio.h>
#include <stdlib.h>

#include "dict.h"



//----------/ CREATE, INIT, DELETE /-----------------------------------------------------------

// creates new dict
Dict* dict_new(uint capacity) {
   // create Dict
   Dict *dict = (Dict*)calloc(1, sizeof(Dict));
   if (!dict) return NULL;

   // create file
   char *filename = dict_generate_name(capacity);
   FILE *fp = fopen(filename, "wb+");
   free(filename);
   if (!fp) {
      free(dict);
      return NULL;
   }

   // init Dict
   dict->size     = 0;
   dict->capacity = capacity;
   dict->fp       = fp;

   // init file
   for (uint i = 0; i < capacity; i++) {
      dict_set(dict, i, 0, 0, 0, 0);
   }

   return dict;
}


// fill dict with 256 single characters;
void dict_init(Dict *dict) {
   if (dict->size != 0) {
      if (DEBUG) printf("Couldn't init not empty dict.\n");
      return;
   }

   for (int i = 0; i < 256; i++) {
      dict_insert(dict, dict->capacity, (char)i);
   }

   // for (int i = 65; i < 91; i++) {
   //    dict_insert(dict, dict->capacity, (char)i);
   // }
   // dict_insert(dict, dict->capacity, 'a');
   // dict_insert(dict, dict->capacity, 'b');
}

// delete dict and its file
void dict_delete(Dict *dict) {
   dict_delete_file(dict);
   free(dict);
}





//----------/ INTERFACE /------------------------------------------------------

// return free index for insertion (busy == 0) or index of found item (busy == 1)
uint dict_find(Dict *dict, uint prefix_index, char character) {
   uint index = dict_hash(dict, prefix_index, character);
   Item item = dict_get(dict, index);

   while (item.busy == 1 && !(item.character == character &&
                              item.prefix_index == prefix_index)) {
      index = dict_iterate(dict, index);
      item = dict_get(dict, index);
   }

   return index;
}

// insert item (includes existence check and extension)
uint dict_insert(Dict *dict, uint prefix_index, char character) {
   uint index = dict_find(dict, prefix_index, character);
   if (dict_get(dict, index).busy == 1) {
      printf("[DUPLICATE: insert (%u, %c) in dict_%u ]\n", prefix_index, character, dict->capacity);
      return index;
   }

   dict_set(dict, index, 1, prefix_index, character, 0);
   dict_set_queue(dict, index);
   dict->size++;


   if (dict->size >= (uint)((dict->capacity)*MAX_FULLNESS)) {
      index = dict_extend(dict);
   }

   return index;
}

// create new file, copy all items to it (returns new index of last inserted item)
uint dict_extend(Dict *dict) {
   uint new_capacity = dict_next_capacity(dict);
   printf("[EXTEND] to %u...\n", new_capacity);
   Dict *new_dict = dict_new(new_capacity);
   if (!new_dict) return dict->capacity;

   Item item;
   Item prefix_item;
   uint index;
   uint new_prefix_index;

   uint new_index;

   // read indexes from queue, insert corresponding items to new_dict with fixed prefix_index'es
   for (uint i = 0; i < dict->size; i++) {
      index = dict_get_queue(dict, i);
      item = dict_get(dict, index);

      // if prefix_index exists, use respective new_prefix_index
      if (item.prefix_index != dict->capacity) {
         prefix_item = dict_get(dict, item.prefix_index);
         new_prefix_index = dict_find(new_dict, prefix_item.new_prefix_index, prefix_item.character);

         dict_set(dict, index, 1, item.prefix_index, item.character, new_prefix_index);

         item.prefix_index = new_prefix_index;

      } else { // if prefix_index doesn't exist, new prefix_index doesn't exist too
         // set new_prefix_index in old dict
         dict_set(dict, index, 1, item.prefix_index, item.character, new_dict->capacity);

         item.prefix_index = new_dict->capacity;
      }

      new_index = dict_insert(new_dict, item.prefix_index, item.character);
   }

   dict_delete_file(dict);
   dict->size     = new_dict->size;
   dict->capacity = new_dict->capacity;
   dict->fp       = new_dict->fp;
   free(new_dict);


   printf("[EXTEND] finished.\n");
   return new_index;
}

// print all items (including not busy) and queue
void dict_print(Dict *dict) {
   char *filename = dict_generate_name(dict->capacity);
   printf("[ Dict: \"%s\" ]\n",
          filename);
   free(filename);

   printf("Items:\n");
   for (uint i = 0; i < dict->capacity; i++) {
      Item item = dict_get(dict, i);
      printf("[%u]\t(%d, %c, %u)\n",
             i, (int)item.busy, item.character, item.prefix_index);
   }

   // printf("Queue:\n");
   // for (uint i = 0; i < dict->size; i++) {
   //    uint index = dict_get_queue(dict, i);
   //    printf("%u\n", index);
   // }

   printf("\n");
}

// print all words;
void dict_print_words(Dict *dict) {
   // . . .
}





//----------/ FILE /-----------------------------------------------------------

// delete corresponding file
void dict_delete_file(Dict *dict) {
   fclose(dict->fp);

   char *filename = dict_generate_name(dict->capacity);
   remove(filename);
   free(filename);
}

// read item from file by index
Item dict_get(Dict *dict, uint index) {
   Item item;
   fseek(dict->fp, index*sizeof(Item), SEEK_SET);
   fread(&item, sizeof(Item), 1, dict->fp);

   if (DEBUG) printf("dict_get: (%d, %c, %u)\n",
                     (int)item.busy, item.character, item.prefix_index);

   return item;
}

// write item to file by index
void dict_set(Dict *dict, uint index, char busy,
              uint prefix_index, char character, uint new_prefix_index) {
   Item item = {.busy = busy, .prefix_index = prefix_index, .character = character, .new_prefix_index = new_prefix_index};
   fseek(dict->fp, index*sizeof(Item), SEEK_SET);
   fwrite(&item, sizeof(Item), 1, dict->fp);

   if (DEBUG) printf("dict_set: (%d, %c, %u)\n",
                     (int)item.busy, item.character, item.prefix_index);
}


// read index from queue by q_index
uint dict_get_queue(Dict *dict, uint q_index) {
   uint index;

   uint offset = (dict->capacity)*sizeof(Item);
   fseek(dict->fp, offset + q_index*sizeof(uint), SEEK_SET);
   fread(&index, sizeof(uint), 1, dict->fp);

   if (DEBUG) printf("dict_get_queue: (%u)\n", index);

   return index;
}

// write index to the end of file, i.e. add it to queue
void dict_set_queue(Dict *dict, uint index) {
   fseek(dict->fp, 0, SEEK_END);
   fwrite(&index, sizeof(uint), 1, dict->fp);

   if (DEBUG) printf("dict_set_queue: (%u)\n", index);
}





//----------/ AUXILIARY /------------------------------------------------------

// create filename for dict using its capacity
char* dict_generate_name(uint capacity) {
   char *name0 = "dict_";
   int   len0  = 5;

   int len = len0;
   if (capacity == 0) len = 1;
   else {
      uint i = capacity;
      while (i > 0) {
         i /= 10;
         len++;
      }
   }

   char *name = (char*)calloc(len + 1, sizeof(char));
   sprintf(name, "%s%u", name0, capacity);
   name[len] = '\0';
   return name;
}

// calculate next capacity value
uint dict_next_capacity(Dict *dict) {
   return (uint)((dict->capacity)*EXTEND_RATIO);
}

// hash
uint dict_hash(Dict *dict, uint prefix_index, char character) {
   uint hash = 23;

   for (int i = 0; i < 17; i++) {
      hash = hash*31 + prefix_index;
      hash = hash*47 + (int)character;
   }
   hash = hash % dict->capacity;

   return hash;
}

// get next probe of index
uint dict_iterate(Dict *dict, uint index) {
   uint next_index = index + 1;
   if (next_index >= dict->capacity)
      next_index -= dict->capacity;
   return next_index;
}
