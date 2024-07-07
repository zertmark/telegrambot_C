#include "bot.h"
#include "commands.h"
#define SIZE_OF_ARRAY(array) (sizeof(array) / sizeof(array[0]))
#define MESSAGE_REPLY_SIZE 4096
#define INVALID_ARGS_SIZE 48
telebot_handler_t handle;
static telebot_error_e error_status;

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
char* getHelp()
{
    if (helpMessage)
    {
        return helpMessage;
    }
    FILE *file_ptr = fopen("help.txt","r");
    fseek(file_ptr, 0, SEEK_END);
    int f_size = ftell(file_ptr);
    rewind(file_ptr);
    helpMessage = malloc(sizeof(char)*f_size); //this is dangerous way to read file
    for(int i = 0; i < f_size; i++)
    {
        helpMessage[i] = fgetc(file_ptr);
    }
    helpMessage[f_size] = '\0';
    fclose(file_ptr);
    return helpMessage;
}
int isValidFunction(char **args, int argsLen, int *commandId)
{
    for (int c=0;c<commandsCount;c++)
    {
        if(!strcmp(commands[c].name, args[0]))
        {
            *commandId = c;
            //printf("argsLen:%d\n", commands[c].args_len_min <= argsLen && argsLen <= commands[c].args_len_max);
            return (commands[c].args_len_min <= argsLen && argsLen <= commands[c].args_len_max &&
            	   fieldsAreNotSQLCommands(args+1, argsLen));
        }
    }
    return 0;
}
char* getReplyFromDatabase(char *text)
{
    int argsCount = 0;
    int commandId = 0;
    char** args = splitString(text,&argsCount," ", 10);
    //printf("[DEBUG] Command ID:%d\targs[0]: %s\targs[1]: %s", commandId, args[0], args[1]);
    //printf("ARGS: ")
    //TO:DO Fix recognition of commands
    if (!isValidFunction(args, --argsCount, &commandId))
    {
        for (int c=0;c<argsCount;c++)
        {
            free(args[c]);
        }
        char* buffer = malloc(sizeof(char)*(INVALID_ARGS_SIZE+strlen(commands[0].name)));
	    sprintf(buffer, "Invalid args\nPlease type '%s' to see commands\n", 
						  commands[0].name);
	    return buffer;
    }
    //for(int c=0;c<SIZE_OF_ARRAY(commands[commandId].argsTypes) && commands[commandId].argsTypes[c] != NONE;c++)
    //{
    //TO:DO Add all commands for bot
    switch (commandId)
    {
        case 0:
        case 1:
            return getHelp();
            break;
	    case 2:
            if (argsCount==2)
            {
                return ((char *(*)(char*, int)) commands[commandId].func_pointer)(args[1],atoi(args[2]));
            }
            return ((char *(*)(char*, int)) commands[commandId].func_pointer)(args[1],10);
            break;

        default:
	        printf("Didn't found command with %d ID\n", commandId);
            break;
    }
    printf("FUCK\n");
    //}
    //printf("%d\n", SIZE_OF_ARRAY(commands[commandId].argsTypes));
    return NULL;
}
void startBot()
{
    //TO:DO Rewrite to telebot-core
    printf("Welcome to Echobot\n");
    //printf("%d\n",commands[0].argsTypes[1]);
    int index, count, offset = -1;
    telebot_message_t message;
    telebot_update_type_e update_types[] = {TELEBOT_UPDATE_TYPE_MESSAGE};
    char message_reply[MESSAGE_REPLY_SIZE]= {0};
    openDatabase("/root/Desktop/FinanceBot/databases/test.db");
    //telebot_core_response_t *response;
    //commands[0].test();
    //if (executeReadCommand("SELECT * FROM FINANCE;") == 0)
    //{
    //    printf("FUCK\n");
    //    //printf("ERROR: %s\n", errMessage);
    //    closeDatabase();
    //    return;
    //}
    //printBuffer();
    //freeBuffer();
    //printf("%s\n", getHelp());
    
    //printf("%d\n", commands[0].test);
    //printf("%s\n", ((char *(*)(char *))commands[0].test) ("HASKI"));
    //printf("test1\n");   
    while (1)
    {
        telebot_update_t *updates;
        //error_status = telebot_core_get_updates(handle, offset,20,0,"message", response);
        error_status = telebot_get_updates(handle, offset, 20, 0, update_types, 0, &updates, &count);
        
        if (error_status != TELEBOT_ERROR_NONE)
        {
            printf("Error occured: %d\n", error_status);
            continue;
	    }
        for (index = 0; index < count; index++)
        {
            char *reply = NULL;
            message = updates[index].message;
            if (!message.text)
            {
                offset = updates[index].update_id + 1;
                continue;
            }
            reply = getReplyFromDatabase(message.text);
	        if (reply == NULL || strlen(reply) > MESSAGE_REPLY_SIZE)
	        {
	    	    printf("[ERROR] Didn't found command in commands or reply is too big\n");
		        offset = updates[index].update_id + 1;
		        if (reply != NULL)
			        free(reply);
	    	    continue;
	        }
//            if (strlen(reply) <= MESSAGE_REPLY_SIZE)
//            {
//                printf("Got right response...\nSending reply message...\n");
            strncpy(message_reply, reply, strlen(reply));
                //snprintf(message_reply, SIZE_OF_ARRAY(message_reply), "%s", reply);
	        printf("%s\n", message_reply);
//            }
            //printf("%s\n", message_reply);
            //strncpy(message_reply, getReplyFromDatabase(message.text), )
            //printf("%s\n", getReplyFromDatabase(message.text));
            //if (strstr(convertToLowerCase(message.text), "/start1"))
            //{
            //    snprintf(message_reply, MESSAGE_REPLY_SIZE, "Hello %s", message.from->first_name);
            //}
            error_status = telebot_send_message(handle, message.chat->id, message_reply, "Markdown", false, false, updates[index].message.message_id, "");
            //error_status = telebot_core_send_message()
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
    telebot_destroy(handle);
    closeDatabase();
}
