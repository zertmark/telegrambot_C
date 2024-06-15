#include "database.h"
#include "stack.h"
#include "finance.h"
#include  "bot.h"
typedef enum standTypes
{
    NONE,
    INT,
    FLOAT,
    STR
} types;

typedef struct commandBasic
{
    char name[20];
    void (*func_pointer)(void);
    types argsTypes[10];
    int args_len_min;
    int args_len_max;
} command;

command commands[] = 
{
    {
        "/помощь",
        (void(*)(void))getHelp,
        {NONE},
        0,
        0
    },
    {
        "/start",
        (void(*)(void))getHelp,
        {NONE},
        0,
        0
    },
    {
        "/пм",
        (void(*)(void))getMonthProfit,
        {STR, NONE},
        1,
        1
    }
};