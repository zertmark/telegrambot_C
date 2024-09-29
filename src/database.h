#pragma once
#include <sqlite3.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stab.h>
#include <stddef.h>
#include <string.h>

#define TABLE_HEADERS_STRING_STACK "ID\tNAME\tREMAINING\tCOST\tREVENUE\tPROFIT\tPROFIT %\tCOST_1"
#define TABLE_HEADERS_STRING_STACK_LENGTH 61

#define NUMBER_OF_HEADERS_STACK 8
#define NUMBER_OF_HEADERS_FINANCE 4

#define TABLE_HEADERS_STRING_FINANCE "ID\tNAME\tPLAN\tPROFIT"
#define TABLE_HEADERS_STRING_FINANCE_LENGHT 28

#define        DEFAULT_PATH "./databases/d1.db"
#define        MAX_STRING_LINES 100

/*  Strange idea to implement mechanism to know, 
    what kind of table names existing database should have 
*/
#define        TABLE_NAME_1 "STACK"
#define        TABLE_NAME_2 "FINANCE"

int fieldsAreNotSQLCommands(char **fieldsList, int sizeList);
int fieldAndRowExist(char *dataBaseTableName, char *field, char *field_data);
int openDatabase(char *path_to_database);
void printBuffer(void);
int executeWriteCommand(char *command_string);
int executeReadCommand(char *command_string);
sqlite3* getStack(void);
char** fetchall(void);
char* fetchone(char **buffer, int *iter);
void freeBuffer(void);
char* revealDatabase(char *dataBaseTableName, int number_of_lines);
void freeTable(void** table_ptr, unsigned long long int size);
size_t getBufferRowsCount(void);
float getFieldsAverageSum(char *dataBaseTableName, char *field);
float getFieldsSum(char *dataBaseTableName, char *field);
int updateDatabaseRowsCount(char *dataBaseTableName);
void closeDatabase(void);
char** splitString(char *argsIn, int *listArgsSize, const char* determinator, int maxNumberOfStrings);