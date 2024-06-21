#include "database.h"
#include "stack.h"
#include "finance.h"
#include  "bot.h"
//typedef enum standTypes
//{
//    NONE,
//    INT,
//    FLOAT,
//    STR
//} types;

typedef struct commandBasic
{
    char name[20];
    void (*func_pointer)(void);
    int args_len_min;
    int args_len_max;
} command;

command commands[] = 
{
    {
        "/помощь",
        (void(*)(void)) getHelp,
        0,
        0
    },
    {
        "/start",
        (void(*)(void)) getHelp,
        0,
        0
    },
    {
        "/пм",
        (void(*)(void)) getMonthProfit,
        1,
        1
    },
    {
        "/п",
        (void(*)(void)) getCurrentMonthProfit,
        0,
        0
    }
//    {
//        "/пм",
//        (void(*)(void))getMonthProfit,
//        1,
//        1
//    },
//    {
//        "/пм",
//        (void(*)(void))getMonthProfit,
//        1,
//        1
//    },
//    {
//        "/пм",
//        (void(*)(void))getMonthProfit,
//        1,
//        1
//    },
//    {
//        "/пм",
//        (void(*)(void))getMonthProfit,
//        1,
//        1
//    },
//    {
//        "/пм",
//        (void(*)(void))getMonthProfit,
//        1,
//        1
//    },
//    {
//        "/пм",
//        (void(*)(void))getMonthProfit,
//        1,
//        1
//    },
};
