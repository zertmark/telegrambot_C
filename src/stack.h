#include <math.h>
#include <string.h>
#define TABLE_HEADERS_STRING "ID\tNAME\tREMAINING\tCOST\tREVENUE\tPROFIT\tPROFIT_PROCENT\tCOST_1"
#define TABLE_HEADERS_STRING_LENGTH 67
#define NUMBER_OF_HEADERS 8
float getCompleteRevenue(void);
float getCompleteProfit(void);
float getCompleteCost(void);
float getAverageCostOne(void);
float getCompeleteProfitProcent(void);
char* searchProductInfo(char *args);
int addNewProduct(char *name, int remaining, int cost, int revenue, int profit, float profit_procent, int cost_1);


static const char *headersTable[] = {
    "ID",
    "Name",
    "Remaining",
    "Cost",
    "Revenue",
    "Profit",
    "Profit_procent",
    "Cost_1"
};
typedef struct {
    int product_id;
    char *name;
    int remaining;
    int cost;
    int revenue;
    int profit;
    float profit_procent;
    int cost_1;
} row;

float getCompleteRevenue(void)
{
    return getFieldsSum("STACK", "revenue");
}
float getCompleteCost(void)
{
    return getFieldsSum("STACK", "cost");
}
float getAverageCostOne(void)
{
    return getFieldsAverageSum("STACK", "cost_1");
}
float getCompleteProfit(void)
{
    return getCompleteRevenue() - getCompleteCost();
}
float getCompeleteProfitProcent(void)
{
    return round((double) getCompleteRevenue() / getCompleteCost())*100;
}
//TO:DO Fix this function
//static char** getFieldsList(char *rowString)
//{
//    char **outputList = NULL;
//    char *field = strtok(rowString, " ");
//    size_t iter = 0;
//    while(field)
//    {
//        printf("%s ", field);
//        outputList[iter]=malloc(strlen(field)+1);
//        strcpy(outputList[iter], field);
//        iter++;
//    }
//    return outputList;
//}
char* getProductIDString(char *name)
{
    char formatted_command[128] = {0};
    sprintf(formatted_command, "SELECT product_id, name FROM STACK WHERE name = %s;\0", name);
    if(sqlite3_complete(name) || !executeReadCommand(formatted_command))
    {
        return NULL;
    }
    char **results = fetchall();
    char *output = malloc(strlen(results[0])+1);
    strncpy(output, results[0], strlen(results[0])+1);
    freeBuffer();
    return output;
}
int deteleProduct(int product_id)
{   
    if (product_id < 0)
    {
        return 0;
    }
    char *formatted_command[128] = {0};
    sprintf(formatted_command, "DELETE FROM STACK WHERE product_id = %d;\n", product_id);
    return executeWriteCommand(formatted_command);
}
char* getMostProfitableProduct(void)
{ 
    if(!executeReadCommand("SELECT product_id, name, remaining, cost, max(revenue), profit, profit_procent, cost_1 FROM STACK;"))
    {
        return NULL;
    }
    char **resultRows = fetchall();
    char *output = malloc(strlen(TABLE_HEADERS_STRING)+strlen(resultRows[0])+2);
    sprintf(output, "%s\n%s", TABLE_HEADERS_STRING, resultRows[0]);
    freeBuffer();
    return output;
}
static char** splitString(char *argsIn, int *listArgsSize, const char* determinator)
{
    int iter = 0;
    char buffer[256] = {0};
    char **outputList = malloc(sizeof(char *) * NUMBER_OF_HEADERS);
    strncpy(buffer, argsIn,256);
    char *buffer_ptr = strtok(buffer, determinator);
    
    while(buffer_ptr != NULL && iter != NUMBER_OF_HEADERS)
    {
        outputList[iter] = malloc(sizeof(char) * (strlen(buffer_ptr)+1));
        strncpy(outputList[iter], buffer_ptr, strlen(buffer_ptr)+1);
        buffer_ptr = strtok(NULL, determinator);
        iter++;
    }
    free(buffer_ptr);
    *listArgsSize = iter;
    return outputList;
}
char* getProductInfo(int id, char *field)
{
    char command[128] = {0};
    sprintf(command, "SELECT product_id, name, %s FROM STACK WHERE product_id = %d;", field, id);
    if(sqlite3_complete(field) || !executeReadCommand(command))
    {
        return NULL;
    }
    char **resultRows = fetchall();
    char *output = malloc(strlen(field)+strlen(resultRows[0])+strlen("product_id, name\n"));
    sprintf(output, "%s\n%s", "ID\tNAME\tFIELD\t", resultRows[0]);
    freeBuffer();
    return output;
}
char* searchProductInfo(char *args)
{
    int listArgsSize = 0; 
    char command[512] = "SELECT product_id, name, remaining, cost, revenue, profit, profit_procent, cost_1 FROM STACK WHERE ";
    char **fields = splitString(args, &listArgsSize, " =");
    if(!fieldsAreNotSQLCommands(fields, listArgsSize))
    {
        return NULL;
    }
    free(fields);
    fields = splitString(args, &listArgsSize, " ");
    for (int c=0;c<listArgsSize-1;c++)
    {
        strcat(command, fields[c]);
        strcat(command, " AND ");
    }
    strcat(command, fields[listArgsSize-1]);
    strcat(command, ";");
    if(!executeReadCommand(command))
    {
        return NULL;
    }
    char **resultRows = fetchall();
    char *outputString = malloc(TABLE_HEADERS_STRING_LENGTH+strlen(resultRows[0])+1);
    sprintf(outputString, "%s\n%s", TABLE_HEADERS_STRING, resultRows[0]);
    freeBuffer();
    free(fields);
    return outputString;
}
//static int addNewDataToRow(row *newRowLine, char **fields)
//{
//    newRowLine->remaining+=fields
//}
static void copyStringIntoRow(char **arrayOfFields, row *outputRow)
{
    char *buffer = malloc(strlen(arrayOfFields[1])+1);
    strncpy(buffer, arrayOfFields[1], strlen(arrayOfFields[1])+1);

    outputRow->product_id = atoi(arrayOfFields[0]);
    outputRow->name = buffer;
    outputRow->remaining = atoi(arrayOfFields[2]);
    outputRow->cost =    atoi(arrayOfFields[3]);
    outputRow->revenue = atoi(arrayOfFields[4]);
    outputRow->profit = atoi(arrayOfFields[5]);
    outputRow->profit_procent = atof(arrayOfFields[6]);
    outputRow->cost_1 = atoi(arrayOfFields[7]);
}
static void addRows(row *oldRow, row *newRow)
{
    newRow->product_id = oldRow->product_id;
    newRow->remaining += oldRow->remaining;
    newRow->cost += oldRow->cost;
    newRow->revenue += oldRow->revenue;
    newRow->profit = newRow->revenue - newRow->cost;
    newRow->profit_procent = round(newRow->revenue / newRow->cost)*100;
    newRow->cost_1 = oldRow->cost_1;
}
int addNewProduct(char *name, int remaining, int cost, int revenue, int profit, float profit_procent, int cost_1)
{   
    if(sqlite3_complete(name))
    {
        return 0;
    }
    char command [512] = {0};
    int fieldsCount = 0;
    row oldRowLine;
    row newRowLine = {0, name, remaining, cost, revenue, profit, profit_procent, cost_1};
    if (!fieldAndRowExist("STACK", "name", name))
    {
        sprintf(command, "INSERT INTO STACK (name, remaining, cost, revenue, profit, profit_procent, cost_1) VALUES (%s, %d, %d, %d, %d, %f, %d);", name, remaining, cost, revenue, profit, profit_procent, cost_1);
        return executeWriteCommand(command);
    }
    sprintf(command, "SELECT product_id, name, remaining, cost, revenue, profit, profit_procent, cost_1 FROM STACK WHERE name = %s;", name);
    if(!executeReadCommand(command))
    {
        return 0;
    }
    char **currentLines = fetchall();
    char **currentFields = splitString(currentLines[0], &fieldsCount, "\t");
    copyStringIntoRow(currentFields, &oldRowLine);
    addRows(&oldRowLine, &newRowLine);
    freeTable(currentFields, fieldsCount);
    freeBuffer();

    sprintf(command, "UPDATE STACK SET remaining = %d, cost = %d, revenue = %d, profit = %d," \ 
    "profit_procent = %lf, cost_1 = %d  WHERE product_id = %d;\0", newRowLine.remaining, 
                newRowLine.cost, newRowLine.revenue, newRowLine.profit,
                newRowLine.profit_procent, newRowLine.cost_1, newRowLine.product_id);
    
    return executeWriteCommand(command);

}
