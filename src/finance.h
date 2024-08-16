#pragma once
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h> 
int getPlanForYear(void);
char* getCurrentMonth(void);
char* getMonthProfitID(char *monthName);
char* getMonthProfit(char *monthName);
char* getCurrentMonthPlan(void);
char* getMonthPlan(char *monthName);
char* getCurrentMonthProfit(void);
int setPlanForMonth(char *monthName, int newPlan);
int setProfitForMonth(char *monthName, int newProfit);
