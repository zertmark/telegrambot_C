#include "token_reader.h"
static void read_token_in_term(char* token)
{
	/*
	This function doesn't work properly 
	Need's to be fixed but i gonna rewrite this project from scratch anyway
	*/
	scanf("%ms", &token);
	token[TOKEN_LENGHT] = 0;
}
char* get_token()
{
    FILE *file_ptr = fopen(".token","r");

	char* token = calloc(TOKEN_LENGHT, sizeof(char));

	if ((file_ptr) == NULL)
	{
		printf("File .token wasn't found\n"
			   "Please enter token:");

		read_token_in_term(token);
		return token;	
	}
    fseek(file_ptr, 0, SEEK_END);

    size_t f_size = ftell(file_ptr);
    rewind(file_ptr);
    
    for(size_t i = 0; i < f_size; i++)
    {
        token[i] = fgetc(file_ptr);
    }

    token[f_size] = 0;
    fclose(file_ptr);
    return token;
}
