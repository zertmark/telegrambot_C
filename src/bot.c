#include "bot.h"
#include "commands.h"
#define SIZE_OF_ARRAY(array) (sizeof(array) / sizeof(array[0]))
//telebot_handler_t handle;
telebot_core_handler_t handle;
static telebot_error_e error_status;
#define MESSAGE_REPLY_SIZE 4096

char* helpMessage = NULL;
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
    return (telebot_core_create(&handle, token) == TELEBOT_ERROR_NONE);
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
            return (commands[c].args_len_min <= argsLen && argsLen <= commands[c].args_len_max);
        }
    }
    return 0;
}
char *getReplyFromDatabase(char *text)
{   
    int argsCount = 0;
    int commandId = 0;
    char** args = splitString(text,&argsCount," ", 10);
    if (!isValidFunction(args, --argsCount, &commandId))
    {
        free(args);
        return "Invalid args\nPlease type '/помощь' to see commands\n";
    }
    //for(int c=0;c<SIZE_OF_ARRAY(commands[commandId].argsTypes) \
              && commands[commandId].argsTypes[c] != NONE;c++)
    //{
    switch (commandId)
    {
        case 0:
        case 1:
            return getHelp();
            break;

        default:
            break;
    }
    //}
    //printf("%d\n", SIZE_OF_ARRAY(commands[commandId].argsTypes));
    return "Test";

}
void startBot(char *token)
{
    //TO:DO Rewrite to telebot-core
    printf("Welcome to Echobot\n");
    //printf("%d\n",commands[0].argsTypes[1]);
    int index, count, offset = -1;
    telebot_message_t message;
    telebot_update_type_e update_types[] = {TELEBOT_UPDATE_TYPE_MESSAGE};
    char message_reply[MESSAGE_REPLY_SIZE]= {0};
    telebot_core_response_t *response;
    //commands[0].test();
    //openDatabase("/root/Desktop/FinanceBot/databases/test.db");
    //if (executeReadCommand("SELECT * FROM STACK;") == 0)
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
    char *reply = NULL;
    while (1)
    {
        //telebot_update_t *updates;
        //error_status = telebot_core_get_updates(handle, offset,20,0,update_types,  response);
        //error_status = telebot_get_updates(handle, offset, 20, 0, update_types, 0, &updates, &count);
        
        if (error_status != TELEBOT_ERROR_NONE)
            //printf("Error occured: %d\n", error_status);
            continue;
        printf("Response%s\n", response);
        for (index = 0; index < count; index++)
        {
            //message = updates[index].message;
            //if (!message.text)
            //{
                offset = updates[index].update_id + 1;
                continue;
            //}
            //printf("%s: %s \n", message.from->first_name, message.text);
            //telebot_send_dice(handle, message.chat->id, false, 0, "");
            //reply = getReplyFromDatabase(message.text);
            //printf("%d\n", strlen(reply));
            //if (strlen(reply) <= MESSAGE_REPLY_SIZE)
            //{
            //    printf("Got right response...\nSending reply message...\n");
                //strncpy(message_reply, reply, strlen(reply));
            //    snprintf(message_reply, SIZE_OF_ARRAY(message_reply), "%s", reply);
            //}
            //printf("%s\n", message_reply);
            //strncpy(message_reply, getReplyFromDatabase(message.text), )
            //printf("%s\n", getReplyFromDatabase(message.text));
            //if (strstr(convertToLowerCase(message.text), "/start1"))
            //{
            //    snprintf(message_reply, MESSAGE_REPLY_SIZE, "Hello %s", message.from->first_name);
            //}
            //error_status = telebot_send_message(handle, message.chat->id, message_reply, "HTML", false, false, updates[index].message.message_id, "");
            //error_status = telebot_core_send_message()
            if (error_status != TELEBOT_ERROR_NONE)
            {
                printf("Failed to send message to %s: %d \n", message.from->first_name, error_status);
            }
            
            offset = updates[index].update_id + 1;
        }
        //telebot_put_updates(updates, count);
        telebot_core_put_response(response);
        memset(message_reply, 0, 4096);
        sleep(1);
    }
    telebot_core_destroy(handle);
    //telebot_destroy(handle);
}