#include <stdlib.h>
#include <stdio.h>

#include "dict.h"
#include "lzw.h"


const char DEBUG = 0;


int main(int argc, const char** argv) {
   char * filename_in  = "input.txt";
   char * filename_out = "output.txt";
   char * filename_re  = "reset.txt";

   // +???
   //encode
   FILE *fp_in = fopen(filename_in, "r+");
   if (!fp_in) return 1;

   FILE *fp_out = fopen(filename_out, "wb+");
   if (!fp_out) return 1;

   // FILE *fp_re = fopen(filename_re, "w+");
   // if (!fp_out) return 1;

   lzw_encode(fp_in, fp_out);
   //lzw_decode(fp_out, fp_re);
   // lzw_decode(fp_out, stdout);
   // decode
   // FILE *fp_in = fopen(filename_in, "rb+");
   // if (!fp_in) return 1;

   // FILE *fp_out = fopen(filename_out, "w+");
   // if (!fp_out) return 1;

   // lzw_decode(fp_in, fp_out);

   fclose(fp_in);
   fclose(fp_out);

   // Dict *dict = dict_new(INIT_CAPACITY);
   // dict_init(dict);

   // unsigned long prefix_index = dict_find(dict, dict->capacity, 'A');
   // printf("a index: %lu\n", prefix_index);
   // for (int i = 0; i < 11000; i++) {
   //    prefix_index = dict_insert(dict, prefix_index, 'b');
   //    if (prefix_index == dict->capacity) {
   //       printf("\nerror\n");
   //       break;
   //    }
   // }
   // dict_print(dict);

   // dict_delete(dict);
   return 0;
}


// void print_usage() {
// 	printf ("\nUsing: lzw -[e|d] infile outfile\n");
// 	printf ("-e - encoding (compression)\n");
// 	printf ("-d - decoding (decompression)\n");
// }


// long file_size(char *filename) {
//    FILE *fp = fopen(filename, "r");
//    if (!fp) return 0;

//    fseek(fp, 0, SEEK_END);
//    long size = ftell(fp);

//    fclose(fp);
//    return size;
// }
