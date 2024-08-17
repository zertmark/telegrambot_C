#include "bot.h"
#include "commands.h"
#include "stack.h"
#include "telebot-types.h"
#include <stdio.h>
#include <string.h>
#include <threads.h>
#include <time.h>
#define SIZE_OF_ARRAY(array) (sizeof(array) / sizeof(array[0]))
#define MESSAGE_REPLY_SIZE 4096
#define INVALID_ARGS_SIZE 48
#define INVALID_COMMAND_SIZE 21
#define GR_COMMAND_STRING_OUTPUT 21
#define DL_COMMAND_STRING_OUTPUT_OK 12
#define DL_COMMAND_STRING_OUTPUT_ERROR 29
#define ADD_COMMAND_STRING_OUTPUT_OK 10
#define ADD_COMMAND_STRING_OUTPUT_ERROR 17
#define ST_COMMNAND_STRING_OUTPUT_OK 18 
#define ST_COMMAND_STRING_OUTPUT_ERROR 26
telebot_handler_t handle;
telebot_error_e error_status;

static char* helpMessage = NULL;
static int commandsCount = SIZE_OF_ARRAY(commands);

static char* convertToLowerCase(char *string)
{
    for(int i = 0; string[i]; i++)
    {
        string[i] = tolower(string[i]);
    }
    return string;
}
int tryTocreateBot(char *token)
{
    return (telebot_create(&handle, token) == TELEBOT_ERROR_NONE);
}
static void readHelpMessage()
{
    FILE *file_ptr = fopen("help.txt","r");
    fseek(file_ptr, 0, SEEK_END);
    size_t f_size = ftell(file_ptr);
    rewind(file_ptr);
    helpMessage = calloc(f_size, sizeof(char)); //this is dangerous way to read file
    for(size_t i = 0; i < f_size; i++)
    {
        helpMessage[i] = fgetc(file_ptr);
    }
    helpMessage[f_size] = 0;
    fclose(file_ptr);
}
static void free_args(char** args, int argsCount)
{
    for (int c=0;c<argsCount;c++)
    {
        if (args[c]!=NULL)
            free(args[c]);
    }
    free(args);
}
char* getHelp()
{
    if (!helpMessage)
    {
        readHelpMessage();    
    }
    char* output = calloc(strlen(helpMessage), sizeof(char));
    strncpy(output, helpMessage, strlen(helpMessage));
    return output;
}
int isValidFunction(char **args, int argsLen, int *commandId)
{
    if (args[0]==NULL)
    {
        return 0;
    }
    for (int c=0;c<commandsCount;c++)
    {
        if(!strcmp(commands[c].name, args[0]))
        {
            *commandId = c;
            return (commands[c].args_len_min <= argsLen && argsLen <= commands[c].args_len_max &&
            	   fieldsAreNotSQLCommands(args+1, argsLen));
        }
    }
    return 0;
}
void stopBot()
{
    telebot_destroy(handle);
    free(helpMessage);
    closeDatabase();
}
char* getReplyFromDatabase(char *text)
{
    int argsCount = 0;
    int commandId = 0;
    printf("Got message\n");
    char** args = splitString(text,&argsCount," ", 10);
    //TO:DO Fix recognition of commands
    if (!isValidFunction(args, --argsCount, &commandId))
    {
        char* buffer = calloc(INVALID_ARGS_SIZE+strlen(commands[0].name), sizeof(char));
	    sprintf(buffer, "Invalid args\nPlease type '%s' to see commands\n", 
						  commands[0].name);
        free_args(args, argsCount);
	    return buffer;
    }
    // Awful code and the way of doing memory managment
    //TO:DO Add all commands for bot
    char* c_output = NULL;
    int c_result = 0;
    float c_float = 0.0;
    switch (commandId)
    {
        case 0:
        case 1:
            return getHelp();
            break;
        case 2:
            if (argsCount==2)
            {
                c_output = ((char *(*)(char*, int)) commands[commandId].func_pointer)(args[1],atoi(args[2]));
                break;
            }
            c_output = ((char *(*)(char*, int)) commands[commandId].func_pointer)(args[1],10);
            break;
        case 3:
            c_output = calloc(5, sizeof(char));
            strcpy(c_output, "Stop");
            break;
        case 4:
            c_output = ((char *(*)(char**, int)) commands[commandId].func_pointer)(args+1,argsCount);
            break;
        case 5:
            c_output = ((char *(*)(void)) commands[commandId].func_pointer)();
            break;
        case 6:
        case 7:
            c_float = ((float (*)(void)) commands[commandId].func_pointer)();
            c_output = calloc(sizeof(float)+GR_COMMAND_STRING_OUTPUT,sizeof(char));
            sprintf(c_output, "Total: %.3f", c_float);
            break;
        case 8:
            c_result =((int (*)(int)) commands[commandId].func_pointer)(atoi(args[1]));
            if (c_result)
            {
                c_output = calloc( DL_COMMAND_STRING_OUTPUT_OK+strlen(args[1]),sizeof(char));
                sprintf(c_output, "Deleted %s\n", args[1]);
                break;
            }
            c_output = calloc(DL_COMMAND_STRING_OUTPUT_ERROR+strlen(args[1]),sizeof(char));
            sprintf(c_output, "Couldn't delete %s\n", args[1]);
            break;
        case 9:
            c_result =((int (*)(char*, int, int, int, int, float, int)) 
                                        commands[commandId].func_pointer)
                                        (args[1], atoi(args[2]), atoi(args[3]),
                                         atoi(args[4]), atoi(args[5]), atof(args[6]),
                                         atoi(args[7]));
            if (c_result)
            {
                c_output = calloc( ADD_COMMAND_STRING_OUTPUT_OK+strlen(args[1]),sizeof(char));
                sprintf(c_output, "Added %s\n", args[1]);
                break;
            }
            c_output = calloc(ADD_COMMAND_STRING_OUTPUT_ERROR+strlen(args[1]),sizeof(char));
            sprintf(c_output, "Couldn't add %s\n", args[1]);
            break;
        case 10:
        case 11:
            c_output = ((char* (*)(void)) commands[commandId].func_pointer)();
            break;
        case 12:
        case 13:
            c_output = ((char* (*)(char *)) commands[commandId].func_pointer)(args[1]);
            break;
        case 14:
        case 15:
            c_result = ((int (*) (char*, int)) commands[commandId].func_pointer)(args[1], atoi(args[2]));
            if (c_result)
            {
                c_output = calloc(ST_COMMNAND_STRING_OUTPUT_OK+strlen(args[1]), sizeof(char));
                sprintf(c_output, "Updated month %s\n", args[1]);
                break;
            }
            c_output = calloc(ST_COMMAND_STRING_OUTPUT_ERROR+strlen(args[1]), sizeof(char));
            sprintf(c_output, "Couldn't update month %s\n", args[1]);
            break;
        default:
            c_output = calloc(INVALID_COMMAND_SIZE, sizeof(char));
	        strncpy(c_output, "Didn't found command", INVALID_COMMAND_SIZE);
            break;
    }
    free_args(args, argsCount);
    return c_output;
}
void startBot()
{
    int index = 0, count = 0, offset = -1;
    telebot_message_t message;
    telebot_update_type_e update_types[] = {TELEBOT_UPDATE_TYPE_MESSAGE};
    char message_reply[MESSAGE_REPLY_SIZE]= {0};
    openDatabase("/root/Desktop/FinanceBot/databases/test.db");   
    while (1)
    {
        telebot_update_t *updates;
        error_status = telebot_get_updates(handle, offset,100, 0, update_types, 0, &updates, &count);
        if (error_status != TELEBOT_ERROR_NONE && error_status != TELEBOT_ERROR_OPERATION_FAILED)
        {
            printf("Error occured: %d\n", error_status);
            continue;
	    }
        for (index = 0; index < count; index++)
        {
            if(updates[index].update_type != TELEBOT_UPDATE_TYPE_MESSAGE)
            {
                offset = updates[index].update_id + 1;
                continue;
            }
            char *reply = NULL;
            message = updates[index].message;
            if (!message.text)
            {
                offset = updates[index].update_id + 1;
                printf("ERROR\n");
                continue;
            }
            reply = getReplyFromDatabase(message.text);

	        if (reply == NULL || strlen(reply) > MESSAGE_REPLY_SIZE)
	        {
	   	        printf("[ERROR] Couldn't execute command or reply is too big\n");
		        offset = updates[index].update_id + 1;
		        if (reply != NULL)
		    	    free(reply);
	        	continue;
	        }
            if (!strcmp(reply, "Stop"))
            {
                printf("Stopping\n");
                error_status = telebot_send_message(handle, message.chat->id,"Stopping bot\n", "Markdown", false, false, updates[index].message.message_id, "");
                telebot_put_updates(updates, count);
                stopBot();
                exit(0);
            }
            printf("Got right response...\nSending reply message...\n");
            strncpy(message_reply, reply, strlen(reply));
	        printf("%s\n", message_reply);
            error_status = telebot_send_message(handle, message.chat->id, message_reply, "Markdown", false, false, updates[index].message.message_id, "");
            if (error_status != TELEBOT_ERROR_NONE)
            {
                printf("Failed to send message to %s: %d \n", message.from->first_name, error_status);
            }
            offset = updates[index].update_id + 1;
	        free(reply);
        }
        telebot_put_updates(updates, count);
        memset(message_reply, 0, 4096);
        sleep(1);
    }
    stopBot();
    exit(0);
}
