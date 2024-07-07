#pragma once
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
char* getProductIDString(char *name);
int deteleProduct(int product_id);
char* getMostProfitableProduct(void);
float getCompleteRevenue(void);
float getCompleteProfit(void);
float getCompleteCost(void);
float getAverageCostOne(void);
float getCompeleteProfitProcent(void);
char* searchProductInfo(char *args);
int addNewProduct(char *name, int remaining, int cost, int revenue, 
                    int profit, float profit_procent, int cost_1);
char* searchProductInfo(char *args);