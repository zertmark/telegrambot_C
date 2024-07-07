#include "database.h"
#define        DEFAULT_PATH "/root/Desktop/FinanceBot/databases/test.db"
#define        MAX_STRING_LINES 100
static sqlite3  *dataBase = {0};
static char     *errMessage = NULL;
static char     **buffer = NULL;
static int      bufferRowsCount = 0;
static int      databaseRowsCount = 0;
static char     *primaryKey = "product_id";

char** splitString(char *argsIn, int *listArgsSize, 
                            const char* determinator, 
                            int maxNumberOfStrings)
{
    int iter = 0;
    char **outputList = malloc(sizeof(char *) * maxNumberOfStrings);
    char buffer_t[512] = {0};
    strncpy(buffer_t, argsIn,512);
    char *buffer_ptr = strtok(buffer_t, determinator);
    
    while(buffer_ptr != NULL && iter != maxNumberOfStrings)
    {
        outputList[iter] = malloc(sizeof(char) * (strlen(buffer_ptr)+1));
        strncpy(outputList[iter], buffer_ptr, strlen(buffer_ptr));
        outputList[iter][strlen(buffer_ptr)] = 0;
        buffer_ptr = strtok(NULL, determinator);
        iter++;
    }
    free(buffer_ptr);
    *listArgsSize = iter;
    return outputList;
}
static int get_buffer_size(int number_of_lines)
{
    if (buffer==NULL || bufferRowsCount==0 || number_of_lines==0)
    {
        printf("FUCK\n");
        return 0;
    }
    int buffer_size = 0;
    for(int c=0;c<number_of_lines;c++)
    {
        buffer_size+=strlen(buffer[c])+1;
    }
    printf("buffer size:%d\n",buffer_size);
    return buffer_size;
}
void freeTable(void** table_ptr, unsigned long long int size)
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
//    sprintf(formatted_command, "UPDATE %s SET %s = %s WHERE %s = %d;", dataBaseTableName, field, new_info, primaryKey, id);
//    return executeWriteCommand(formatted_command);
//}
int fieldAndRowExist(char *dataBaseTableName, char *field, char *field_data)
{
    char formatted_command[256] = {0};
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
    char formatted_command[128] = {0}; 
    float sum = 0.0;
    sprintf(formatted_command, "SELECT %s FROM %s;", field, dataBaseTableName);
    if (!executeReadCommand(formatted_command))
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

char* revealDatabase(char* dataBaseTableName, int number_of_lines)
{
    //TO:DO Fix this function
    char formatted_command[128] = {0};
    char *output = NULL;
    sprintf(formatted_command, "SELECT * FROM %s;", dataBaseTableName);
    printf("formatted_command:%s\tnumber of lines:%d\n", formatted_command, number_of_lines);
    if(!executeReadCommand(formatted_command))
    {
        return output;
    }
    number_of_lines = number_of_lines > bufferRowsCount ? bufferRowsCount: number_of_lines;
    int buffer_size = get_buffer_size(number_of_lines)+TABLE_HEADERS_STRING_STACK_LENGTH+7;
    output = calloc(buffer_size, sizeof(char));
    strcpy(output, "```\n");
    if (!strcmp(dataBaseTableName, "STACK"))
    {
        strncat(output, TABLE_HEADERS_STRING_STACK, TABLE_HEADERS_STRING_STACK_LENGTH);
    }
    if (!strcmp(dataBaseTableName, "FINANCE"))
    {
        strncat(output, TABLE_HEADERS_STRING_FINANCE, TABLE_HEADERS_STRING_FINANCE_LENGHT);
    }
    for(int c=0;c<number_of_lines; c++)
    {
        strcat(output, buffer[c]);
        strcat(output, "\n");
    }
    strcat(output, "```");
    output[buffer_size] = 0;
    freeBuffer();
    return output;
}
float getFieldsSum(char* dataBaseTableName, char* field)
{
    char formatted_command[128] = {0}; 
    float sum = 0.0f;
    sprintf(formatted_command, "SELECT %s FROM %s;", field, dataBaseTableName);
    if (!executeReadCommand(formatted_command))
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
    int count = 0;
    
    //for(count = 0; sqlite3_step(stmt)==SQLITE_ROW; count++);
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
    for(int c=0;c<bufferRowsCount;c++)
    {
        printf("%s\n", buffer[c]);
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
    buffer = malloc(bufferRowsCount * sizeof(char*));
    int iter = 0;
    char row_line [1024] = {0};
    while(sqlite3_step(stmt) == SQLITE_ROW)
    {
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
