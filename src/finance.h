#include <time.h>
#define TABLE_HEADERS_FINANCE_STRING "ID\tNAME\tPLAN\tREAL_PROFIT"
#define TABLE_HEADERS_FINANCE_STRING_LENGTH 28
#define NUMBER_OF_HEADERS_FINANCE 4 
int getPlanForYear(void);
const char* getCurrentMonth(void);
char* getMonthProfitID(char *monthName);
char* getMonthProfit(char *monthName);
char* getCurrentMonthPlan(void);
char* getMonthPlan(char *monthName);
char* getCurrentMonthProfit(void);
int setPlanForMonth(char *monthName, int newPlan);
int setProfitForMonth(char *monthName, int newProfit);

static time_t time_s = {0};
static struct tm* currentTime = {0};
static const char *months [] = {
    "January",
    "February",
    "April",
    "March",
    "May",
    "June",
    "July",
    "August",
    "Septembers",
    "October",
    "November",
    "December"
};
int getPlanForYear(void)
{
    return getFieldsSum("FINANCE", "plan");
}
const char* getCurrentMonth(void)
{
    time_s = time(NULL);
    currentTime = localtime(&time_s);
    return months[currentTime->tm_mon];
}
char* getMonthProfitID(char *monthName)
{
    char command [128];
    sprintf(command, "SELECT month_id, real_profit FROM FINANCE WHERE name = '%s';", monthName);
    if(sqlite3_complete(monthName) || !executeReadCommand(command))
    {
        return NULL;
    }
    char **outputLine = fetchall();
    char *output = malloc(19+strlen(outputLine[0]));
    sprintf(output, "ID\tREAL_PROFIT\n%s\n", outputLine[0]);
    freeBuffer();
    free(outputLine);
    return output;
}
char* getMonthProfit(char *monthName)
{
    char command [128];
    sprintf(command, "SELECT real_profit FROM FINANCE WHERE name = '%s';", monthName);
    if(sqlite3_complete(monthName) || !executeReadCommand(command))
    {
        return NULL;
    }
    char **outputLine = fetchall();
    char *output = malloc(strlen(outputLine[0])+strlen(monthName)+20);
    sprintf(output, "Profit of month %s:\n%s\n", monthName, outputLine[0]);
    freeBuffer();
    free(outputLine);
    return output;
}
char* getMonthPlan(char *monthName)
{
    char command [128];
    sprintf(command, "SELECT plan FROM FINANCE WHERE name = '%s';", monthName);
    if(sqlite3_complete(monthName) || !executeReadCommand(command))
    {
        return NULL;
    }
    char **outputLine = fetchall();
    char *output = malloc(strlen(outputLine[0])+strlen(monthName)+20);
    sprintf(output, "Plan for month %s:\n%s\n", monthName, outputLine[0]);
    freeBuffer();
    free(outputLine);
    return output;
}
char* getCurrentMonthPlan(void)
{
    return getMonthPlan(getCurrentMonth());
}
char* getCurrentMonthProfit(void)
{
    return getMonthProfit(getCurrentMonth());
}
int setPlanForMonth(char *monthName, int newPlan)
{
    char command[128] = {0};
    sprintf(command, "UPDATE FINANCE SET plan = %d WHERE name = '%s';\n", newPlan, monthName);
    return executeWriteCommand(command);
}
int setProfitForMonth(char *monthName, int newProfit)
{
    char command[128] = {0};
    sprintf(command, "UPDATE FINANCE SET real_profit = %d WHERE name = '%s';\n", newProfit, monthName);
    return executeWriteCommand(command);
}