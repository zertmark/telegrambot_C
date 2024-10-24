#include "bot.h"
#include "token_reader.h"
int main()
{
    if (!tryTocreateBot(get_token()))
    {
    	printf("Failed to login with token\nExiting...\n");
    	return -1;
    }
    printf("Starting bot...\n");
    startBot();
    return 0;
}