#include "bot.h"
#define TOKEN "ENTER_YOUR_TOKEN_HERE"

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
