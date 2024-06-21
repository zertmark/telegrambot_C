#include "bot.h"
#define TOKEN "5766970401:AAHWeiRf_AMi1Z4p7hZ8_est7pi1665-iQY"

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
