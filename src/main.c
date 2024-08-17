#include "bot.h"
#define TOKEN "5766970401:AAGTrrHEL3BIO7sj7T2oYGMxXB-FsdF6zmA"

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
