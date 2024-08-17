#include "bot.h"
#define TOKEN "5766970401:AAF8xYXEQ0yaPJEr4VCOFaBNHVO2jgUei0Y"

int main(int argc, char *argv[])
{
    if (!tryTocreateBot(TOKEN))
    {
    	printf("Failed to login with token\nExiting...\n");
    	return -1;
    }
    startBot();
    return 0;
}
