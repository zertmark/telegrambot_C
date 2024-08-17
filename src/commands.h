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
        "/help",
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
        "/show",
        (void(*)(void)) revealDatabase,
        1,
        2
    },
    {
        "/stop",
        (void(*)(void)) stopBot,
        0,
        0
    },
    {
        "/search",
        (void(*)(void)) searchProductInfo,
        1,
        NUMBER_OF_HEADERS_STACK*2
    },
    {
        "/gmp",
        (void(*)(void)) getMostProfitableProduct,
        0,
        0
    },
    {
        "/gr",
        (void(*)(void)) getCompleteRevenue,
        0,
        0
    },
    {
        "/gcpr",
        (void(*)(void)) getCompleteProfit,
        0,
        0
    },
    {
        "/delete",
        (void(*)(void)) deteleProduct,
        1,
        1
    },
    {
        "/add",
        (void(*)(void)) addNewProduct,
        1,
        NUMBER_OF_HEADERS_STACK
    },
    {
        "/gcmpr",
        (void(*)(void)) getCurrentMonthProfit,
        0,
        0
    },
    {
        "/gcmp",
        (void(*)(void)) getCurrentMonthPlan,
        0,
        0
    },
    {
        "/gmpr",
        (void(*)(void)) getMonthProfit,
        1,
        1
    },
    {
        "/gmp",
        (void(*)(void)) getMonthPlan,
        1,
        1
    },
    {
        "/splfm",
        (void(*)(void)) setPlanForMonth,
        2,
        2
    },
    {
        "/sprfm",
        (void(*)(void)) setProfitForMonth,
        2,
        2
    }
};
