#include <sqlite3.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stab.h>
#include <stddef.h>
#include <string.h>
#pragma once
int fieldsAreNotSQLCommands(char **fieldsList, int sizeList);
int fieldAndRowExist(char *dataBaseTableName, char *field, char *field_data);
void openDatabase(char *path_to_database);
void printBuffer(void);
int executeWriteCommand(char *command_string);
int executeReadCommand(char *command_string);
sqlite3* getStack(void);
char** fetchall(void);
char* fetchone(char **buffer, int *iter);
void freeBuffer(void);
void freeTable(void** table_ptr, unsigned long long int size);
int getBufferRowsCount(void);
float getFieldsAverageSum(char *dataBaseTableName, char *field);
float getFieldsSum(char *dataBaseTableName, char *field);
int updateDatabaseRowsCount(char *dataBaseTableName);
void closeDatabase(void);
char** splitString(char *argsIn, int *listArgsSize, const char* determinator, int maxNumberOfStrings);
