#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stab.h>
#include "database.h"
#include "stack.h"
#include "finance.h"
#define        DEFAULT_PATH "/root/Desktop/FinanceBot/databases/test.db"
#define        MAX_STRING_LINES 100
//#define setRowInfo(dataBaseTableName, id, field, new_info) _Generic (a )
static sqlite3  *dataBase = {0};
static char     *errMessage = NULL;
static char     **buffer = NULL;
static int      bufferRowsCount = 0;
static int      databaseRowsCount = 0;
static char     *primaryKey = "product_id";
//static char    *dataBaseTableName = "TEST";


void freeTable(void** table_ptr, size_t size)
{
    for(int c=0;c<size;c++)
    {
        free(table_ptr[c]);
    }
    free(table_ptr);
}
void freeBuffer(void)
{
    if (buffer==NULL)
    {
        bufferRowsCount = 0;
        return;
    }
    for(int c=0;c<bufferRowsCount;c++)
    {
        free(buffer[c]);
    }
    buffer = NULL;
    bufferRowsCount = 0;
}
int fieldsAreNotSQLCommands(char **fieldsList, int sizeList)
{
    for(int c=0;c<sizeList;c++)
    {
        if(sqlite3_complete(fieldsList[c]))
        {
            return 0;
        }
    }
    return 1;
}
//int setRowInfo(char *dataBaseTableName, int id, char *field, void *new_info, char *format)
//{
//    char formatted_command[256] = {0};
//    if (sqlite3_complete(field) || sqlite3_complete(new_info))
//    {
//        return 0;
//    }
//    sprintf(formatted_command, "UPDATE %s SET %s = %s WHERE %s = %d;\0", dataBaseTableName, field, new_info, primaryKey, id);
//    return executeWriteCommand(formatted_command);
//}
int fieldAndRowExist(char *dataBaseTableName, char *field, char *field_data)
{
    char formatted_command[256] = {0};
    int output = 0;
    sprintf(formatted_command, "SELECT EXISTS (SELECT 1 FROM %s WHERE %s = %s);", dataBaseTableName, field, field_data);
    if (sqlite3_complete(field) || sqlite3_complete(field_data) || !executeReadCommand(formatted_command))
    {
        return output;
    }
    output = atoi(buffer[0]);
    freeBuffer();
    return output;
}
float getFieldsAverageSum(char *dataBaseTableName, char *field)
{
    char formatted_command[128] = {0}; 
    float sum = 0.0;
    sprintf(formatted_command, "SELECT %s FROM %s;", field, dataBaseTableName);
    if (sqlite3_complete(field) || !executeReadCommand(formatted_command))
    {
        return 0.0;
    }
    for(int c=0;c<bufferRowsCount;c++)
    {
        sum+=atoi(buffer[c]);
    }
    freeBuffer();
    return sum/bufferRowsCount;
}

char** revealDatabase(char *dataBaseTableName, int number_of_lines)
{
    char formatted_command[128] = {0};
    char **output = NULL;
    sprintf(formatted_command, "SELECT * FROM %s;", dataBaseTableName);
    if(!executeReadCommand(formatted_command))
    {
        return output;
    }
    number_of_lines = number_of_lines > bufferRowsCount ? bufferRowsCount: number_of_lines;
    output = malloc(sizeof(char *)*number_of_lines);
    for(int c=0;c < number_of_lines; c++)
    {
        output[c] = malloc(strlen(buffer[c])+1);
        strcpy(output[c], buffer[c]);
    }
    
    freeBuffer();
    bufferRowsCount=number_of_lines;
    return output;
}
float getFieldsSum(char *dataBaseTableName, char *field)
{
    char formatted_command[128] = {0}; 
    float sum = 0.0f;
    sprintf(formatted_command, "SELECT %s FROM %s;\0", field, dataBaseTableName);
    if (sqlite3_complete(field) || !executeReadCommand(formatted_command))
    {
        return 0.0f;
    }
    for(int c=0;c<bufferRowsCount;c++)
    {
        sum+=atof(buffer[c]);
    }
    freeBuffer();
    return sum;
}
int updateDatabaseRowsCount(char *dataBaseTableName)
{
    char formatted_command[128] = {0};   
    sprintf(formatted_command, "SELECT %s FROM %s;", primaryKey, dataBaseTableName);
    if(!executeReadCommand(formatted_command))
    {
        return 0;
    }
    databaseRowsCount = bufferRowsCount;
    freeBuffer();
    return 1;
}
int getDatabaseRowsCount(void)
{
    return databaseRowsCount;    
}
int getBufferRowsCount(void)
{
    return bufferRowsCount;
}
void openDatabase(char *path_to_database)
{
    sqlite3_initialize();
    if (access(path_to_database, F_OK || W_OK || R_OK) == -1) 
    {
        path_to_database = DEFAULT_PATH;
    }
    sqlite3_open(path_to_database, &dataBase);    
}
static int countRowsResult(sqlite3_stmt *stmt)
{   
    sqlite3_reset(stmt);
    size_t count  = 0;
    while(sqlite3_step(stmt)==SQLITE_ROW)
    {
        count++;
    }
    return count;
}
static int countColumnResult(sqlite3_stmt *stmt)
{
    sqlite3_reset(stmt);
    if(sqlite3_step(stmt)!=SQLITE_ROW)
    {
        return 0;
    }
    sqlite3_reset(stmt);
    return sqlite3_column_count(stmt);
}
char** fetchall(void)
{
    return buffer;
}
char* fetchone(char **buffer, int *iter)
{ 
    return buffer[*iter];
} 
void printBuffer(void)
{
    for(int c=0;c<bufferRowsCount;c++)
    {
        printf(buffer[c]);
        printf("\n");
    }
}
int executeReadCommand(char *command_string)
{
    if (!sqlite3_complete(command_string)) 
    {
        errMessage ="SQL COMMAND ERROR: Incomplete command\n";
        return 0;
    }
    sqlite3_stmt *stmt;
    sqlite3_prepare_v2(dataBase, command_string, strlen(command_string)+1, &stmt, NULL);
    bufferRowsCount = countRowsResult(stmt);
    //freeBuffer();
    buffer = malloc(bufferRowsCount * sizeof(char*));
    int iterator = 0;
    char row_line [1024] = {0};
    while(sqlite3_step(stmt) == SQLITE_ROW)
    {
        int column_count = sqlite3_column_count(stmt); 
        memset(row_line, 0, 1024);
        for(int c=0;c<column_count;c++)
        {
            if (sqlite3_column_type(stmt, c)!=SQLITE_NULL)
            {
                strcat(row_line, sqlite3_column_text(stmt, c));
                strcat(row_line, "\t");
                continue;
            }
            strcat(row_line, "NULL");
            strcat(row_line, "\t");
        }
        strcat(row_line, "\0");
        buffer[iterator] = malloc(strlen(row_line)+1);
        strcpy(buffer[iterator], row_line);
        iterator++;
    }
    sqlite3_finalize(stmt);
    return 1;
}
int executeWriteCommand(char *command_string)
{
    if (!sqlite3_complete(command_string)) 
    {
        errMessage ="SQL COMMAND ERROR: Incomplete command\n";
        return 0;
    }
    errMessage = NULL;
    sqlite3_exec(dataBase,  command_string, NULL, 0, &errMessage);
    if (errMessage != NULL)
    {
        printf("SQL COMMAND ERROR: %s\n", errMessage);
        return 0;
    }
    return 1;
}
sqlite3* getStack(void)
{
    return dataBase;
}
char* getErrorMessage(void)
{
    return errMessage;
}
int main()
{
    openDatabase("/root/Desktop/FinanceBot/databases/test.db");
    //executeWriteCommand("INSERT INTO FINANCE (month_id, name, plan, real_profit) VALUES (12, 'December', 12000, 5000);");
    printf("%s\n", getCurrentMonthPlan());
    //executeWriteCommand("CREATE TABLE IF NOT EXISTS FINANCE(" 
    //"month_id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT," 
    //"name TEXT NOT NULL," 
    //"plan INTEGER NOT NULL," 
    //"real_profit INTEGER NOT NULL);");

    //executeWriteCommand("DROP TABLE TEST;");
    //char *command = "CREATE TABLE IF NOT EXISTS STACK("  \
    //"product_id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, " \ 
    //"name TEXT NOT NULL, " \
    //"remaining INTEGER NOT NULL," \
    //"cost INTEGER NOT NULL," \
    //"revenue INTEGER," \
    //"profit INTEGER," \
    //"profit_procent REAL," \
    //"cost_1  INTEGER);";
    //executeWriteCommand(command);
    //executeWriteCommand("INSERT INTO STACK (product_id, name, remaining, cost, revenue, profit, profit_procent, cost_1) VALUES (2, 'HQD', 100, 30000, 45000, 15000, 50, 300);");
    //char *command = "SELECT * FROM TEST;";
    //if (executeReadCommand("SELECT * FROM STACK;") == 0)
    //{
    //    printf("ERROR: %s\n", errMessage);
    //    sqlite3_close(dataBase);
    //    return -1;
    //}
    //printBuffer();
    //freeBuffer();
    //printf("getCompleteCost: %f\n", getCompleteCost());
    //printf("getCompleteRevenue:%f\n", getCompleteRevenue());
    //printf("getCompleteProfit:%f\n", getCompleteProfit());
    //printf("getCompleteProfitProcent:%f\n", getCompeleteProfitProcent());
    //printf("getMostProfitableProduct:\n%s\n", getMostProfitableProduct());
    //printf("getProducIDString:%s\n", getProductIDString("'HQD'"));
    ////setRowInfo("STACK", 4, "revenue", 5000);
    //addNewProduct("'Brusko'", 10, 3000, 4000, 1000, 0.25f, 300);
    //deteleProduct(3);
    //printf(getProductInfo(1, "name"));
    //char *test_str = malloc(sizeof(char)*28);
    //test_str = "name='HQD'";    
    //printf(searchProductInfo(test_str));
    //printf("\nSEG?\n");
    //free(test_str);
    //char **test = revealDatabase(5)
    //for(int c=0;c<getBufferRowsCount();c++)
    //{
    //    printf(test[c]);
    //    printf("\n");
    //}
    //char **test = fetchall();
    //strcpy(test, fetchall());
    //for(int c=0;c<=sizeof(test)/sizeof(test[0]);c++)
    //{
    //    printf("%s\n", fetchone(test, &c));
    //}
    //printBuffer();
    //freeBuffer();
    //if (executeReadCommand("SELECT * FROM TEST WHERE ID = 3;") == 0)
    //{
    //    printf("ERROR: %s\n", errMessage);
    //    sqlite3_close(dataBase);
    //    return -1;
    //}
    //printBuffer();
    //freeBuffer();
    //printf("%d\n", updateDatabaseRowsCount() ? databaseRowsCount : 0);
    //printf("%f\n", getFieldsAverageSum("SALARY"));
    //printf("%d\n", fieldAndRowExist("STACK", "NAME", "'test'"));

    //printf("Changed database\n");
    sqlite3_close(dataBase);
    return 0;
}