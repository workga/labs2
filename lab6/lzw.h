#ifndef __LZW_H
#define __LZW_H

void lzw_encode(FILE *fp_in, FILE *fp_out);
void lzw_decode(FILE *fp_in, FILE *fp_out);
void write_word(Dict *dict, uint index, FILE *fp_out);

#endif