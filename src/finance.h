#pragma once
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define TABLE_HEADERS_FINANCE_STRING "ID\tNAME\tPLAN\tREAL_PROFIT"
#define TABLE_HEADERS_FINANCE_STRING_LENGTH 28
#define NUMBER_OF_HEADERS_FINANCE 4 
int getPlanForYear(void);
char* getCurrentMonth(void);
char* getMonthProfitID(char *monthName);
char* getMonthProfit(char *monthName);
char* getCurrentMonthPlan(void);
char* getMonthPlan(char *monthName);
char* getCurrentMonthProfit(void);
int setPlanForMonth(char *monthName, int newPlan);
int setProfitForMonth(char *monthName, int newProfit);