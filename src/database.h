#include <sqlite3.h>
#include <stddef.h>
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
char** splitString(char *argsIn, int *listArgsSize, const char* determinator, int maxNumberOfStrings);
//int setRowInfo(char *dataBaseTableName, int id, char *field, char *new_info);