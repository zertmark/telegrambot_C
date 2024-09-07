#include "bot.h"
char* read_token()
{
    FILE *file_ptr = fopen(".token","r");
    fseek(file_ptr, 0, SEEK_END);
    size_t f_size = ftell(file_ptr);
    rewind(file_ptr);
    char* token = calloc(f_size, sizeof(char)); //this is dangerous way to read file
    for(size_t i = 0; i < f_size; i++)
    {
        token[i] = fgetc(file_ptr);
    }
    token[f_size] = 0;
    fclose(file_ptr);
    return token;
}
int main(int argc, char *argv[])
{
    if (!tryTocreateBot(read_token()))
    {
    	printf("Failed to login with token\nExiting...\n");
    	return -1;
    }
    startBot();
    return 0;
}