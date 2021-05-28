// LZW.cpp
/*---------------------------------------------------------
* Алгоритм LZW. Демонстрационная программа.
* Запуск:
*     LZW e|d infile outfile
*---------------------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef unsigned char uchar;
typedef unsigned long ulong;
typedef unsigned int  uint;

/*---------------------------------------------------------
   Побитовый доступ к файлам
*/

typedef struct bfile
{
    FILE *file;
    uchar mask;
    int rack;
    int pacifier_counter;
}
BFILE;

#define PACIFIER_COUNT 2047

BFILE *OpenInputBFile ( char *name );
BFILE *OpenOutputBFile ( char *name );
void  WriteBit  ( BFILE *bfile, int bit );
void  WriteBits ( BFILE *bfile, ulong code, int count );
int   ReadBit  ( BFILE *bfile );
ulong ReadBits ( BFILE *bfile, int bit_count );
void  CloseInputBFile ( BFILE *bfile );
void  CloseOutputBFile ( BFILE *bfile );

/*---------------------------------------------------------
   Функции высокого уровня
*/

void CompressFile ( FILE *input, BFILE *output );
void ExpandFile   ( BFILE *input, FILE *output );
void usage_exit   ( char *prog_name );
void print_ratios ( char *input, char *output );
long file_size    ( char *name );

/*---------------------------------------------------------
   Функции работы с моделью данных для алгоритма LZW
*/

uint find_dictionary_match ( int prefix_code, int character );
uint decode_string ( uint offset, uint code );

/*---------------------------------------------------------
   Константы, используемые при работе LZW
*/
/* Количество битов в коде */
#define BITS                       12
/* Максимальное значение кода */
#define MAX_CODE                   ( ( 1 << BITS ) - 1 )
/* Размер словаря в элементах */
#define TABLE_SIZE                 5021
/* Специальный код конца потока */
#define END_OF_STREAM              256
/* Значение кода, которое получает первая добавленная
в словарь фраза */
#define FIRST_CODE                 257
/* Признак свободной ячейки в словаре */
#define UNUSED                     -1

// имя файла-компрессора
char *compressor_filname = "LZW.exe";

/*-----------------------------------------------------------
   Обработка фатальной ошибки при работе программы.
*/

void fatal_error( char *str, ... )
{
   printf( "Fatal error: %s\n", str );
   exit(1);
}

/*-----------------------------------------------------------
   Открытие файла для побитовой записи
*/

BFILE *OpenOutputBFile ( char * name )
{
   BFILE *bfile;

   bfile = (BFILE *) calloc( 1, sizeof( BFILE ) );
   bfile->file = fopen( name, "wb" );
   bfile->rack = 0;
   bfile->mask = 0x80;
   bfile->pacifier_counter = 0;
   return bfile;
}

/*-----------------------------------------------------------
   Открытие файла для побитового чтения
*/

BFILE *OpenInputBFile( char *name )
{
   BFILE *bfile;

   bfile = (BFILE *) calloc( 1, sizeof( BFILE ) );
   bfile->file = fopen( name, "rb" );
   bfile->rack = 0;
   bfile->mask = 0x80;
   bfile->pacifier_counter = 0;
   return bfile;
}

/*-----------------------------------------------------------
   Закрытие файла для побитовой записи
*/

void CloseOutputBFile ( BFILE *bfile )
{
   if ( bfile->mask != 0x80 )
      putc( bfile->rack, bfile->file );
   fclose ( bfile->file );
   free ( (char *) bfile );
}

/*-----------------------------------------------------------
   Закрытие файла для побитового чтения
*/

void CloseInputBFile ( BFILE *bfile )
{
    fclose ( bfile->file );
    free ( (char *) bfile );
}

/*-----------------------------------------------------------
   Вывод одного бита
*/

void WriteBit ( BFILE *bfile, int bit )
{
   if ( bit )
      bfile->rack |= bfile->mask;
   bfile->mask >>= 1;
   if ( bfile->mask == 0 )
   {
      putc( bfile->rack, bfile->file );
      if ( ( bfile->pacifier_counter++ & PACIFIER_COUNT ) == 0 )
         putc( '.', stdout );
      bfile->rack = 0;
      bfile->mask = 0x80;
   }
}

/*-----------------------------------------------------------
   Вывод нескольких битов
*/

void WriteBits( BFILE *bfile, ulong code, int count )
{
   ulong mask;

   mask = 1L << ( count - 1 );
   while ( mask != 0)
   {
      if ( mask & code )
  bfile->rack |= bfile->mask;
      bfile->mask >>= 1;
      if ( bfile->mask == 0 )
      {
  putc( bfile->rack, bfile->file );
  if ( ( bfile->pacifier_counter++ & PACIFIER_COUNT ) == 0 )
            putc( '.', stdout );
  bfile->rack = 0;
  bfile->mask = 0x80;
      }
      mask >>= 1;
   }
}

/*-----------------------------------------------------------
   Ввод одного бита
*/

int ReadBit( BFILE *bfile )
{
   int value;

   if ( bfile->mask == 0x80 )
   {
      bfile->rack = getc( bfile->file );
      if ( bfile->rack == EOF )
         fatal_error( "Error in function ReadBit!\n" );
      if ( ( bfile->pacifier_counter++ & PACIFIER_COUNT ) == 0 )
         putc( '.', stdout );
   }

   value = bfile->rack & bfile->mask;
   bfile->mask >>= 1;
   if ( bfile->mask == 0 )
      bfile->mask = 0x80;
   return ( value ? 1 : 0 );
}

/*-----------------------------------------------------------
   Ввод нескольких битов
*/

ulong ReadBits ( BFILE *bfile, int bit_count )
{
   ulong mask;
   ulong return_value;

   mask = 1L << ( bit_count - 1 );
   return_value = 0;
   while ( mask != 0 )
   {
      if ( bfile->mask == 0x80 )
      {
  bfile->rack = getc( bfile->file );
  if ( bfile->rack == EOF )
            fatal_error( "Error in function ReadBits!\n" );
  if ( ( bfile->pacifier_counter++ & PACIFIER_COUNT ) == 0 )
            putc( '.', stdout );
      }
      if ( bfile->rack & bfile->mask )
         return_value |= mask;
      mask >>= 1;
      bfile->mask >>= 1;
      if ( bfile->mask == 0 )
  bfile->mask = 0x80;
   }

   return return_value;
}

/*-----------------------------------------------------------
   Вывод сообщения об использовании
*/

void usage_exit ()
{
 printf ("\n Using: %s e|d infile outfile\n", compressor_filname);
 printf ("e - for encoding (compression)\n");
 printf ("d - for decoding (decompression)\n");
 exit( 0 );
}

/*-----------------------------------------------------------
   Измерение размера файла
*/

long file_size ( char *name )
{
   long eof_ftell;
   FILE *file;

   file = fopen( name, "r" );
   if ( file == NULL )
      return( 0L );
   fseek( file, 0L, SEEK_END );
   eof_ftell = ftell( file );
   fclose( file );
   return eof_ftell;
}

/*-----------------------------------------------------------
   Вывод сообщения о соотношении размеров файлов
*/
void print_ratios( char *input, char *output )
{
   long input_size;
   long output_size;
   int ratio;

   input_size = file_size( input );
   if ( input_size == 0 )
      input_size = 1;
   output_size = file_size( output );
   ratio = 100 - (int) ( output_size * 100L / input_size );
   printf( "\nInput size:        %ld bytes\n", input_size );
   printf( "Output size:       %ld bytes\n", output_size );
   if ( output_size == 0 )
      output_size = 1;
   printf( "Ratio: %d%%\n", ratio );
}

/*-----------------------------------------------------------
   Далее начинается исходный текст собственно алгоритма LZW
*/

/* Структура словаря для алгоритма LZW */

struct dictionary
{
   int code_value;
   int prefix_code;
   char character;
}
dict[TABLE_SIZE];

/* Стек для декодирования */

char decode_stack[TABLE_SIZE];

/*-----------------------------------------------------------
   Процедура сжатия файла
*/

void CompressFile ( FILE *input, BFILE *output )
{
   int next_code, character, string_code;
   uint index, i;

   /* Инициализация */
   next_code = FIRST_CODE;
   for ( i = 0 ; i < TABLE_SIZE ; i++ )
       dict[i].code_value = UNUSED;
   /* Считать первый символ */
   if ( ( string_code = getc( input ) ) == EOF )
       string_code = END_OF_STREAM;
   /* Пока не конец сообщения */
   while ( ( character = getc( input ) ) != EOF )
   {
      /* Попытка найти в словаре пару <фраза, символ> */
      index = find_dictionary_match ( string_code, character );
      /* Соответствие найдено */
      if ( dict[index].code_value != -1 )
         string_code = dict[index].code_value;
      /* Такой пары в словаре нет */
      else
      {
         /* Добавление в словарь */
  if ( next_code <= MAX_CODE )
         {
            dict[index].code_value = next_code++;
            dict[index].prefix_code = string_code;
            dict[index].character = (char) character;
         }
         /* Выдача кода */
         WriteBits( output, (ulong) string_code, BITS );
         string_code = character;
      }
   }
   /* Завершение кодирования */
   WriteBits( output, (ulong) string_code, BITS );
   WriteBits( output, (ulong) END_OF_STREAM, BITS );
}

/*-----------------------------------------------------------
   Процедура декодирования сжатого файла
*/

void ExpandFile ( BFILE *input, FILE *output )
{
   uint next_code, new_code, old_code;
   int character;
   uint count;

   next_code = FIRST_CODE;
   old_code = (uint) ReadBits( input, BITS );
   if ( old_code == END_OF_STREAM )
      return;
   character = old_code;

   putc ( old_code, output );

   while ( ( new_code = (uint) ReadBits( input, BITS ) )
             != END_OF_STREAM )
   {
      /* Обработка возможной исключительной ситуации */
      if ( new_code >= next_code )
      {
         decode_stack[ 0 ] = (char) character;
         count = decode_string( 1, old_code );
      }
      else
         count = decode_string( 0, new_code );

      character = decode_stack[ count - 1 ];
      /* Выдача раскодированной строки */
      while ( count > 0 )
         putc( decode_stack[--count], output );
      /* Обновление словаря */
      if ( next_code <= MAX_CODE )
      {
         dict[next_code].prefix_code = old_code;
         dict[next_code].character = (char) character;
         next_code++;
      }
      old_code = new_code;
   }
}

/*-----------------------------------------------------------
   Процедура поиска в словаре указанной пары <код фразы,
   символ>. Для ускорения поиска используется хеш, получаемый
   из параметров.
*/

uint find_dictionary_match ( int prefix_code, int character )
{
   int index;
   int offset;

   /* Собственно получение значения хеш-функции */
   index = ( character << ( BITS - 8 ) ) ^ prefix_code;
   /* Разрешение возможных коллизий */
   if ( index == 0 )
      offset = 1;
   else
      offset = TABLE_SIZE - index;
   for ( ; ; )
   {
      /* Эта ячейка словаря не использована */
      if ( dict[index].code_value == UNUSED )
         return index;
      /* Найдено соответствие */
      if ( dict[index].prefix_code == prefix_code &&
           dict[index].character == (char) character )
         return index;
      /* Коллизия. Подготовка к следующей попытке ее
         разрешения */
      index -= offset;
      if ( index < 0 )
         index += TABLE_SIZE;
   }
}

/*-----------------------------------------------------------
   Процедура декодирования строки. Размещает символы в стеке,
   возвращает их количество.
*/

uint decode_string ( uint count, uint code )
{
   while ( code > 255 ) /* Пока не встретится код символа */
   {
      decode_stack[count++] = dict[code].character;
      code = dict[code].prefix_code;
   }
   decode_stack[count++] = (char) code;
   return count;
}


//------------------------------------------------------------
// Главная процедура
int _tmain(int argc, _TCHAR* argv[])
{
 setbuf( stdout, NULL );

 // в случае неправильного количества аргументов
 // выводится способ использования программы
 if (argc < 4)
  usage_exit();
 
 // Компрессия:
 if (argv [1] [0] == 'e' || argv [1] [0] == 'E')
 {
  BFILE *output;
  FILE *input;

  // открытие входного файла для чтения
  input = fopen( argv[ 2 ], "rb" );
  if ( input == NULL )
   fatal_error( "Ошибка при открытии %s для ввода\n", argv[ 2 ] );

  // открытие выходного файла для записи
  output = OpenOutputBFile( argv[ 3 ] );
  if ( output == NULL )
   fatal_error( "Ошибка при открытии %s для вывода\n", argv[ 3 ] );
  printf( "\nКомпрессия %s в %s\n", argv[ 2 ], argv[ 3 ] );
  
  // вызов процедуры компрессии
  CompressFile( input, output );

  // закрытие файлов
  CloseOutputBFile( output );
  fclose( input );

  printf( "\nCompression complete." );

  // вывод коэффициента сжатия
  print_ratios( argv[ 2 ], argv[ 3 ] );
 }
 // Декомпрессия:
 else if (argv [1] [0] == 'd' || argv [1] [0] == 'D')
 {
  BFILE *input;
  FILE *output;

  // открытие входного файла для чтения
  input = OpenInputBFile( argv[ 2 ] );
  if ( input == NULL )
   fatal_error( "Error on open %s for read\n", argv[ 2 ] );
  
  // открытие выходного файла для записи
  output = fopen( argv[ 3 ], "wb" );
  if ( output == NULL )
   fatal_error( "Error on open %s for write\n", argv[ 3 ] );

  printf( "\nDecompression %s into %s\n", argv[ 2 ], argv[ 3 ] );
  
  // вызов процедуры декомпрессии
  ExpandFile(input, output );

  // закрытие файлов
  CloseInputBFile( input );
  fclose( output );

  printf( "\nDecompression complete." );
 };

   return 0;

}