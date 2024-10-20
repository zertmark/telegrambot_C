#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <time.h>
#include "database.h"
#include "stack.h"
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
    sprintf(formatted_command, "SELECT product_id, name FROM STACK WHERE name = '%s';", name);
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
    char formatted_command[128] = {0};
    sprintf(formatted_command, "DELETE FROM STACK WHERE product_id = %d;\n", product_id);
    return executeWriteCommand(formatted_command);
}
char* getMostProfitableProduct(void)
{
    if(!executeReadCommand("SELECT product_id, name, remaining, cost, max(revenue)," 
    						"profit, profit_procent, cost_1 FROM STACK;"))
    {
        return NULL;
    }
    char **resultRows = fetchall();
    char *output = malloc(strlen(TABLE_HEADERS_STRING_STACK)+strlen(resultRows[0])+2);
    sprintf(output, "%s\n%s", TABLE_HEADERS_STRING_STACK, resultRows[0]);
    freeBuffer();
    return output;
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
char* searchProductInfo(char** args, int listArgsSize)
{
    char command[512] = "SELECT product_id, name, remaining, cost, revenue, profit, profit_procent, cost_1 FROM STACK WHERE ";
    //char **fields = splitString(args, &listArgsSize, " =", NUMBER_OF_HEADERS_STACK);
    // if(!fieldsAreNotSQLCommands(fields, listArgsSize))
    // {
    //     return NULL;
    // }
    // for(int c=0;c<listArgsSize;c++)
    // {
    //     free(fields[c]);
    // }
    // fields = splitString(args, &listArgsSize, " ", NUMBER_OF_HEADERS_STACK);
    for (int c=0;c<listArgsSize-1;c++)
    {
        strcat(command, args[c]);
        strcat(command, " AND ");
    }
    strcat(command, args[listArgsSize-1]);
    strcat(command, ";");
    if(!executeReadCommand(command))
    {
        return NULL;
    }
    char **resultRows = fetchall();
    char *outputString = malloc(TABLE_HEADERS_STRING_STACK_LENGTH+strlen(resultRows[0])+1);
    sprintf(outputString, "%s\n%s", TABLE_HEADERS_STRING_STACK, resultRows[0]);
    freeBuffer();
    //free(fields);
    return outputString;
}
static void copyStringIntoRow(char **arrayOfFields, row *outputRow)
{
    char *buffer = malloc(strlen(arrayOfFields[1])+1);
    strncpy(buffer, arrayOfFields[1], strlen(arrayOfFields[1])+1);

    outputRow->product_id = atoi(arrayOfFields[0]);
    outputRow->name = buffer;
    outputRow->remaining = atoi(arrayOfFields[2]);
    outputRow->cost = atoi(arrayOfFields[3]);
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
    newRow->profit_procent = round((float) newRow->revenue / (float) newRow->cost) * 100.0;
    newRow->cost_1 = oldRow->cost_1;
}
int addNewProduct(char *name, int remaining, 
                    int cost, int revenue, int profit, 
                    float profit_procent, int cost_1)
{   
    if(sqlite3_complete(name))
    {
        return 0;
    }
    char command [512] = {0};
    int fieldsCount = 0;
    row oldRowLine = {0};
    row newRowLine = {0, name, remaining, cost, revenue, profit, profit_procent, cost_1};
    if (!fieldAndRowExist("STACK", "name", name))
    {
        sprintf(command, "INSERT INTO STACK (name, remaining, cost, revenue, profit, profit_procent," \
            "cost_1) VALUES (%s, %d, %d, %d, %d, %f, %d);", name, remaining, cost, revenue,
            profit, profit_procent, cost_1);
        return executeWriteCommand(command);
    }
    sprintf(command, "SELECT product_id, name, remaining, cost, revenue, profit, profit_procent, cost_1 FROM STACK WHERE name = %s;", name);
    if(!executeReadCommand(command))
    {
        return 0;
    }
    char **currentLines = fetchall();
    if (currentLines[0] == NULL)
    {
        printf("FUCK\n");
        return 0;
    }
    char **currentFields = splitString(currentLines[0], &fieldsCount, "\t", NUMBER_OF_HEADERS_STACK);
    copyStringIntoRow(currentFields, &oldRowLine);
    addRows(&oldRowLine, &newRowLine);
    freeTable( (void**) currentFields, fieldsCount);
    freeBuffer();

    sprintf(command, "UPDATE STACK SET remaining = %d, cost = %d, revenue = %d, profit = %d," \
        "profit_procent = %lf, cost_1 = %d  WHERE product_id = %d;", newRowLine.remaining, 
                newRowLine.cost, newRowLine.revenue, newRowLine.profit,
                newRowLine.profit_procent, newRowLine.cost_1, newRowLine.product_id);
    free(newRowLine.name);
    return executeWriteCommand(command);

}
