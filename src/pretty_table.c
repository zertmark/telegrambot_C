#include "pretty_table.h"
#include <stdio.h>
#include <string.h>
#include <time.h>

void clean_table(char*** table, size_t rows_count, size_t columns_count)
{
    if (table==NULL)
    {
        return;
    }
    for(size_t c=0;c<rows_count;c++) 
    {
        for(size_t i=0;i<columns_count;i++)
        {
            free(table[c][i]);
        }
        free(table[c]);
    }
    free(table);
}
void print_table(char*** table, size_t rows_count, size_t columns_count)
{
    if (table == NULL)
    {
        return;
    }
    for(size_t c=0;c<rows_count;c++)
    {
        for(size_t i=0;i<columns_count;i++)
        {
            printf("%s", table[c][i]);
        }
        printf("\n");
    }
}
static size_t get_table_size(size_t rows_count, size_t column_count, int* column_max)
{
    size_t output = 0;
    for(size_t c=0;c<column_count;c++)
    {
        output += column_max[c];
    }
    return output*rows_count;
}
static void append_spaces_to_table(char*** table, int* column_max, size_t rows_count, size_t columns_count)
{
    for(size_t c=0;c<rows_count;c++)
    {
        for(size_t i=0;i<columns_count;i++)
        {
            size_t size = strlen(table[c][i]);
            table[c][i] = realloc(table[c][i], column_max[i]);
            for(size_t j=size;j<column_max[i];j++) 
            {
                table[c][i][j] = ' ';
            }
            table[c][i][column_max[i]] = 0;
        }
    }
}
static void calculate_columns_max(char*** table,int* column_max, 
                        size_t rows_count, size_t columns_count)
{
    for(size_t c=0;c<rows_count;c++)
    {
        for(size_t j=0;j<columns_count;j++)
        {
            if (table[c][j]==NULL)
            {
                continue;
            }
            column_max[j] = max(column_max[j], strlen(table[c][j])+1);
        }
    }
}
static char*** create_table (char** string, int* column_max, int size,
                    size_t rows_count, size_t columns_count, char* headers)
{
    char*** table = calloc(rows_count, sizeof(char**));
    printf("Adding headers\n");
    table[0] = splitString(headers, &size, 
                "\t", columns_count);
    
    for(size_t c=1;c<rows_count;c++)
    {
        if (string[c-1] == NULL)
        {
            continue;
        }
        table[c] = splitString(string[c-1], &size, "\t", columns_count);   
        free(string[c-1]);
    }
    calculate_columns_max (table, column_max,  rows_count,  columns_count);
    append_spaces_to_table(table, column_max,  rows_count,  columns_count);

    free(column_max);
    
    return table;
}
char*** get_table(char** string, size_t rows_count, size_t columns_count, char* headers)
{
    int* column_max = calloc(columns_count, sizeof(int));
    char*** table = create_table(string, column_max, 0, rows_count, columns_count, headers);
    
    return table;
}
char* get_string_table(char** string, size_t rows_count, size_t columns_count, char* headers, size_t* len)
{
    rows_count+=1;
    printf("Generating column max...\n");
    int*    column_max = calloc(columns_count, sizeof(int));
    printf("Generating table...\n");
    char*** table = create_table(string,column_max, 0, rows_count, columns_count, headers);
    printf("Getting table size...\n");
    size_t  table_size = get_table_size(rows_count, columns_count, column_max)+strlen(headers)+1+8;
    printf("Generating output\n");
    char*   output = calloc(table_size,sizeof(char));
    strncpy(output, "```\n", 4);
    for(size_t c=0;c<rows_count;c++)
    {
        for(size_t j=0;j<columns_count;j++)
        {
            strcat(output, table[c][j]);
        }
        strcat(output, "\n");
    }
    printf("Table size: %d\n", (int) table_size);
    strcat(output, "```\n");
    *len = table_size;
    output[table_size] = 0;
    clean_table(table, rows_count, columns_count);
    return output;
}