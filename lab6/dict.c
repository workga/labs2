#include <stdio.h>
#include <stdlib.h>

#include "dict.h"




//----------/ CREATE, INIT, DELETE /-----------------------------------------------------------

// creates new dict
Dict* dict_new(unsigned long capacity) {
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
   for (unsigned long i = 0; i < capacity; i++) {
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

   for (int i = 65; i < 91; i++) {
      dict_insert(dict, dict->capacity, (char)i);
   }

   // for (int i = 65; i < 91; i++) {
   //    dict_insert(dict, dict->capacity, (char)i);
   // }
   // dict_insert(dict, dict->capacity, 'A');
}

// delete dict and its file
void dict_delete(Dict *dict) {
   dict_delete_file(dict);
   free(dict);
}





//----------/ INTERFACE /------------------------------------------------------

// return free index for insertion (busy == 0) or index of found item (busy == 1)
unsigned long dict_find(Dict *dict, unsigned long prefix_index, char character) {
   unsigned long index = dict_hash(dict, prefix_index, character);
   Item item = dict_get(dict, index);

   while (item.busy == 1 && !(item.character == character &&
                              item.prefix_index == prefix_index)) {
      index = dict_iterate(dict, index);
      item = dict_get(dict, index);
   }

   return index;
}

// insert item (includes existence check and extension)
unsigned long dict_insert(Dict *dict, unsigned long prefix_index, char character) {
   unsigned long index = dict_find(dict, prefix_index, character);
   if (dict_get(dict, index).busy == 1) {
      printf("[DUPLICATE: insert (%lu, %c) in dict_%lu ]\n", prefix_index, character, dict->capacity);
      return index;
   }

   dict_set(dict, index, 1, prefix_index, character, 0);
   dict_set_queue(dict, index);
   dict->size++;


   if (dict->size >= (unsigned long)((dict->capacity)*MAX_FULLNESS)) {
      index = dict_extend(dict);
   }

   return index;
}

// create new file, copy all items to it (returns new index of last inserted item)
unsigned long dict_extend(Dict *dict) {
   unsigned long new_capacity = dict_next_capacity(dict);
   printf("[EXTEND] to %lu...\n", new_capacity);
   Dict *new_dict = dict_new(new_capacity);
   if (!new_dict) return dict->capacity;

   Item item;
   Item prefix_item;
   unsigned long index;
   unsigned long new_prefix_index;

   unsigned long new_index;

   // read indexes from queue, insert corresponding items to new_dict with fixed prefix_index'es
   for (unsigned long i = 0; i < dict->size; i++) {
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
   for (unsigned long i = 0; i < dict->capacity; i++) {
      Item item = dict_get(dict, i);
      printf("(%d, %c, %lu)\n",
             (int)item.busy, item.character, item.prefix_index);
   }

   // printf("Queue:\n");
   // for (unsigned long i = 0; i < dict->size; i++) {
   //    unsigned long index = dict_get_queue(dict, i);
   //    printf("%lu\n", index);
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
Item dict_get(Dict *dict, unsigned long index) {
   Item item;
   fseek(dict->fp, index*sizeof(Item), SEEK_SET);
   fread(&item, sizeof(Item), 1, dict->fp);

   if (DEBUG) printf("dict_get: (%d, %c, %lu)\n",
                     (int)item.busy, item.character, item.prefix_index);

   return item;
}

// write item to file by index
void dict_set(Dict *dict, unsigned long index, char busy,
              unsigned long prefix_index, char character, unsigned long new_prefix_index) {
   Item item = {.busy = busy, .prefix_index = prefix_index, .character = character, .new_prefix_index = new_prefix_index};
   fseek(dict->fp, index*sizeof(Item), SEEK_SET);
   fwrite(&item, sizeof(Item), 1, dict->fp);

   if (DEBUG) printf("dict_set: (%d, %c, %lu)\n",
                     (int)item.busy, item.character, item.prefix_index);
}


// read index from queue by q_index
unsigned long dict_get_queue(Dict *dict, unsigned long q_index) {
   unsigned long index;

   unsigned long offset = (dict->capacity)*sizeof(Item);
   fseek(dict->fp, offset + q_index*sizeof(unsigned long), SEEK_SET);
   fread(&index, sizeof(unsigned long), 1, dict->fp);

   if (DEBUG) printf("dict_get_queue: (%lu)\n", index);

   return index;
}

// write index to the end of file, i.e. add it to queue
void dict_set_queue(Dict *dict, unsigned long index) {
   fseek(dict->fp, 0, SEEK_END);
   fwrite(&index, sizeof(unsigned long), 1, dict->fp);

   if (DEBUG) printf("dict_set_queue: (%lu)\n", index);
}





//----------/ AUXILIARY /------------------------------------------------------

// create filename for dict using its capacity
char* dict_generate_name(unsigned long capacity) {
   char *name0 = "dict_";
   int   len0  = 5;

   int len = len0;
   if (capacity == 0) len = 1;
   else {
      unsigned long i = capacity;
      while (i > 0) {
         i /= 10;
         len++;
      }
   }

   char *name = (char*)calloc(len + 1, sizeof(char));
   sprintf(name, "%s%lu", name0, capacity);
   name[len] = '\0';
   return name;
}

// calculate next capacity value
unsigned long dict_next_capacity(Dict *dict) {
   return (unsigned long)((dict->capacity)*EXTEND_RATIO);
}

// hash
unsigned long dict_hash(Dict *dict, unsigned long prefix_index, char character) {
   unsigned long hash = 23;

   for (int i = 0; i < 17; i++) {
      hash = hash*31 + prefix_index;
      hash = hash*47 + (int)character;
   }
   hash = hash % dict->capacity;

   return hash;
}

// get next probe of index
unsigned long dict_iterate(Dict *dict, unsigned long index) {
   unsigned long next_index = index + 1;
   if (next_index >= dict->capacity)
      next_index -= dict->capacity;
   return next_index;
}