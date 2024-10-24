#pragma once
#include <stdio.h>
#include <stdlib.h>




#define SIZE_OF_ARRAY(array) (sizeof(array) / sizeof(array[0]))
#define max(a,b) (a>b ? a:b)
void print_table(char*** table, size_t rows_count, size_t colums_count);
void clean_table(char*** table, size_t rows_count, size_t colums_count);
char*** get_table(char** string, size_t rows_count, size_t colums_count, char* headers);
char* get_string_table(char** string, size_t rows_count, size_t columns_count, char* headers, size_t* len);
char**   splitString(char *argsIn, int *listArgsSize, const char* determinator, size_t maxNumberOfStrings);