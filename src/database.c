#include <sqlite3.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "database.h"
#include "pretty_table.h"
#include "sql_commands.h"

static sqlite3  *dataBase = {0};
static char     *errMessage = NULL;
static char     **buffer = NULL;
static size_t      bufferRowsCount = 0;
static size_t     databaseRowsCount = 0;
static char     *primaryKey = "product_id";
static char		formatted_command[512] = {0};
	
static void free_outputList(char** outputList, int iter)
{
    for (int i = 0; i < iter; i++) 
    {
        free(outputList[i]);
    }
    free(outputList);
}
char** splitString(char *argsIn, int *listArgsSize, 
                            const char* determinator, 
                            size_t maxNumberOfStrings)
{
    if (argsIn == NULL || determinator == NULL || listArgsSize == NULL) 
    {
        return NULL;
    }
    
    int iter = 0;
    char **outputList = (char**)calloc(maxNumberOfStrings, sizeof(char *));
    char* cpy = calloc(strlen(argsIn)+1, sizeof(char));
    char* cpycpy = cpy;
    if (outputList == NULL) 
    {
        return NULL;
    }
    strncpy(cpy, argsIn, strlen(argsIn)+1);
    char *buffer_ptr = strtok(cpy, determinator);

    while(buffer_ptr != NULL && iter != maxNumberOfStrings)
    {
        size_t buffer_ptr_len = strlen(buffer_ptr)+1;
        
        outputList[iter] = calloc(buffer_ptr_len, sizeof(char));
        if (outputList[iter] == NULL) 
        {
            free_outputList(outputList, iter);
            printf("Error: Couldn't allocat memory");
            exit(-1);
        }
        strncpy(outputList[iter], buffer_ptr, buffer_ptr_len);
        outputList[iter][buffer_ptr_len-1] = 0;
        buffer_ptr = strtok(NULL, determinator);
        iter++;
    }
    free(cpycpy);
    *listArgsSize = iter;
    return outputList;
}
static int get_buffer_size(int number_of_lines)
{
    if (buffer==NULL || bufferRowsCount==0 || number_of_lines==0)
    {
        return 0;
    }
    int buffer_size = 0;
    for(int c=0;c<number_of_lines;c++)
    {
        if (buffer[c])
        {
            buffer_size+=strlen(buffer[c])+1;
        }
    }
    return buffer_size;
}
void freeTable(void** table_ptr, unsigned long long int size)
{
    for(size_t c=0;c<size;c++)
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
    for(size_t c=0;c<bufferRowsCount;c++)
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
int fieldAndRowExist(char *dataBaseTableName, char *field, char *field_data)
{
    int output = 0;
    sprintf(formatted_command, "SELECT EXISTS (SELECT 1 FROM %s WHERE %s = %s);", dataBaseTableName, field, field_data);
    if (!executeReadCommand(formatted_command))
    {
        return output;
    }
    output = atoi(buffer[0]);
    freeBuffer();
    return output;
}
float getFieldsAverageSum(char *dataBaseTableName, char *field)
{
    float sum = 0.0;
    sprintf(formatted_command, "SELECT %s FROM %s;", field, dataBaseTableName);
    if (!executeReadCommand(formatted_command))
    {
        return 0.0;
    }
    for(size_t c=0;c<bufferRowsCount;c++)
    {
        sum+=atoi(buffer[c]);
    }
    freeBuffer();
    return sum/bufferRowsCount;
}
char* revealDatabase(char* dataBaseTableName, size_t number_of_lines)
{
    char *output = NULL;
    size_t buffer_size = 0;
    sprintf(formatted_command, "SELECT * FROM %s;", dataBaseTableName);
    if(!executeReadCommand(formatted_command))
    {
        return output;
    }
    // printBuffer();
    number_of_lines = number_of_lines > bufferRowsCount ? bufferRowsCount: number_of_lines;
    if (!strcmp(dataBaseTableName, "STACK"))
    {
        return get_string_table(buffer, number_of_lines,
										NUMBER_OF_HEADERS_STACK, 
										TABLE_HEADERS_STRING_STACK, 
										&buffer_size);
    }
    if (!strcmp(dataBaseTableName, "FINANCE"))
    {
        return get_string_table(buffer, number_of_lines,
										NUMBER_OF_HEADERS_FINANCE, 
										TABLE_HEADERS_STRING_FINANCE, 
										&buffer_size);
    }
    freeBuffer();
    return output;
}
float getFieldsSum(char* dataBaseTableName, char* field)
{
    float sum = 0.0f;
    sprintf(formatted_command, "SELECT %s FROM %s;", field, dataBaseTableName);
    if (!executeReadCommand(formatted_command))
    {
        return 0.0f;
    }
    for(size_t c=0;c<bufferRowsCount;c++)
    {
        sum+=atof(buffer[c]);
    }
    freeBuffer();
    return sum;
}
int updateDatabaseRowsCount(char *dataBaseTableName)
{
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
size_t getBufferRowsCount(void)
{
    return bufferRowsCount;
}
static int checkRows()
{
    for (size_t c=0;c<bufferRowsCount;c++)
    {
        if (strstr(buffer[c], "NULL")!=NULL)
        {
            return 0;
        }
    }
    return 1;
}
static int checkTable(const char* d_table, const char* HEADERS)
{
    sprintf(formatted_command, "SELECT * FROM %s;", d_table);
    if (!executeReadCommand(formatted_command))
    {
        return 0;
    }
    return (bufferRowsCount!=0 
    		&& (strcmp(buffer[0], HEADERS)==0) 
            && checkRows());
}
static int databaseExists(char* path_to_database)
{
	short result = sqlite3_open(path_to_database, &dataBase);
	if (access(path_to_database, F_OK || W_OK || R_OK) == -1) 
    {
        printf("[WARNING] Couldn't find existing database at %s\n"
        		"Using default database path...\n", path_to_database);
        path_to_database = DEFAULT_PATH;
    }
	if (result == SQLITE_ERROR || dataBase == NULL)
	{
		if (dataBase == NULL)
		{
			printf("[ERROR] Not enough memory\n"
					"Exiting...\n");
			return 0;
		}
		printf("%s\n", sqlite3_errmsg(dataBase));
		return 0;
	}
	return 1;
}
int isDatabaseGood()
{
    return checkTable("STACK",TABLE_HEADERS_STRING_STACK)  
    	   && checkTable("FINANCE", TABLE_HEADERS_STRING_FINANCE);
}
int createDatabase(char* path_to_database)
{
	
	return 	sqlite3_open(path_to_database, &dataBase)!=SQLITE_ERROR 
			&& dataBase!=NULL 
			&& executeWriteCommand(CREATE_STACK_COMMAND) 
		   	&& executeWriteCommand(CREATE_FINANCE_COMMAND);
}
int openDatabase(char *path_to_database)
{
    sqlite3_initialize();
    return ( (databaseExists(path_to_database) && isDatabaseGood() ) 
    		|| createDatabase(path_to_database));
}
static int countRowsResult(sqlite3_stmt *stmt)
{   
    sqlite3_reset(stmt);
    int count = 0;
    while(sqlite3_step(stmt)==SQLITE_ROW)
    {
        count++;
    }
    sqlite3_reset(stmt);
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
    return (char **) buffer;
}
char* fetchone(char **buffer, int *iter)
{
    return buffer[*iter];
}
void closeDatabase(void)
{
    sqlite3_close(dataBase);
}
void printBuffer(void)
{
    for(size_t c=0;c<bufferRowsCount;c++)
    {
        printf("%s \n", buffer[c]);
    }
}
int executeReadCommand(char *command_string)
{
    if (!sqlite3_complete(command_string)) 
    {
        errMessage = "SQL COMMAND ERROR: Incomplete command\n";
        return 0;
    }
    sqlite3_stmt *stmt = {0};
    int column_count = 0;
    sqlite3_prepare_v2(dataBase, command_string, strlen(command_string)+1, &stmt, NULL);
    column_count = sqlite3_column_count(stmt);
    bufferRowsCount = countRowsResult(stmt);
    buffer = calloc(bufferRowsCount, sizeof(char*));
    int iter = 0;
    char row_line [1024] = {0};
    while(sqlite3_step(stmt) == SQLITE_ROW)
    {
        memset(row_line, 0, 1024);
        for(int c=0;c<column_count;c++)
        {
            if (sqlite3_column_type(stmt, c)!=SQLITE_NULL)
            {
                strcat(row_line, (char* )sqlite3_column_text(stmt, c));
                strcat(row_line, "\t");
                continue;
            }
            strcat(row_line, "NULL");
            strcat(row_line, "\t");
        }
        strcat(row_line, "\0");
        buffer[iter] = malloc(strlen(row_line)+1);
        strcpy(buffer[iter], row_line);
        iter++;
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
//int main()
//{
//    openDatabase("/root/Desktop/FinanceBot/databases/test.db");
//    //executeWriteCommand("INSERT INTO FINANCE (month_id, name, plan, real_profit) VALUES (12, 'December', 12000, 5000);");
//    //printf("%s\n", getCompleteRevenue());
//    //executeWriteCommand("CREATE TABLE IF NOT EXISTS FINANCE(" 
//    //"month_id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT," 
//    //"name TEXT NOT NULL," 
//    //"plan INTEGER NOT NULL," 
//    //"real_profit INTEGER NOT NULL);");
//
//    //executeWriteCommand("DROP TABLE TEST;");
//    //char *command = "CREATE TABLE IF NOT EXISTS STACK("  
//    //"product_id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, "  
//    //"name TEXT NOT NULL, " 
//    //"remaining INTEGER NOT NULL," 
//    //"cost INTEGER NOT NULL," 
//    //"revenue INTEGER," 
//    //"profit INTEGER," 
//    //"profit_procent REAL," 
//    //"cost_1  INTEGER);";
//    //executeWriteCommand(command);
//    //executeWriteCommand("INSERT INTO STACK (product_id, name, remaining, cost, revenue, profit, profit_procent, cost_1) VALUES (2, 'HQD', 100, 30000, 45000, 15000, 50, 300);");
//    //char *command = "SELECT * FROM TEST;";
//    if (executeReadCommand("SELECT * FROM STACK;") == 0)
//    {
//        printf("FUCK\n");
//        //printf("ERROR: %s\n", errMessage);
//        sqlite3_close(dataBase);
//        return -1;
//    }
//    printBuffer();
//    freeBuffer();
//    printf("getCompleteCost: %f\n", getCompleteCost());
//    printf("getCompleteRevenue:%f\n", getCompleteRevenue());
//    printf("getCompleteProfit:%f\n", getCompleteProfit());
//    printf("getCompleteProfitProcent:%f\n", getCompeleteProfitProcent());
//    printf("getMostProfitableProduct:\n%s\n", getMostProfitableProduct());
//    printf("getCompleteCost: %f\n", getCompleteCost());
//    printf("getProducIDString:%s\n", getProductIDString("HQD"));
//    printf("%s\n", getMonthPlan("January"));
//    printf("%s\n", getCurrentMonthPlan());
//    printf("%s\n", getCurrentMonthProfit());
//    setPlanForMonth("January", 30000);
//    setProfitForMonth("January", 30000);
//    generateExcelFile("test.xlsx", TABLE_HEADERS_STRING_STACK, NUMBER_OF_HEADERS_STACK);
//    ////setRowInfo("STACK", 4, "revenue", 5000);
//    //addNewProduct("'Brusko'", 10, 3000, 4000, 1000, 0.25f, 300);
//    //deteleProduct(3);
//    //printf(getProductInfo(1, "name"));
//    //char *test_str = malloc(sizeof(char)*28);
//    //test_str = "name='HQD'";    
//    //printf(searchProductInfo(test_str));
//    //printf("\nSEG?\n");
//    //free(test_str);
//    //char **test = revealDatabase(5)
//    //for(int c=0;c<getBufferRowsCount();c++)
//    //{
//    //    printf(test[c]);
//    //    printf("\n");
//    //}
//    //char **test = fetchall();
//    //strcpy(test, fetchall());
//    //for(int c=0;c<=sizeof(test)/sizeof(test[0]);c++)
//    //{
//    //    printf("%s\n", fetchone(test, &c));
//    //}
//    //printBuffer();
//    //freeBuffer();
//    //if (executeReadCommand("SELECT * FROM TEST WHERE ID = 3;") == 0)
//    //{
//    //    printf("ERROR: %s\n", errMessage);
//    //    sqlite3_close(dataBase);
//    //    return -1;
//    //}
//    //printBuffer();
//    //freeBuffer();
//    //printf("%d\n", updateDatabaseRowsCount() ? databaseRowsCount : 0);
//    //printf("%f\n", getFieldsAverageSum("SALARY"));
//    //printf("%d\n", fieldAndRowExist("STACK", "NAME", "'test'"));
//
//    //printf("Changed database\n");
//    sqlite3_close(dataBase);
//    return 0;
//}
