#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <unistd.h>
#include <telebot.h>
#include <ctype.h>
#include "stack.h"
#define SIZE_OF_ARRAY(array) (sizeof(array) / sizeof(array[0]))
static telebot_handler_t handle;
static telebot_error_e error_status;

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
void startBot(char *token)
{
    printf("Welcome to Echobot\n");
    
    int index, count, offset = -1;
    telebot_message_t message;
    telebot_update_type_e update_types[] = {TELEBOT_UPDATE_TYPE_MESSAGE};
    char message_reply[4096];
    while (1)
    {
        telebot_update_t *updates;
        error_status = telebot_get_updates(handle, offset, 20, 0, update_types, 0, &updates, &count);
        if (error_status != TELEBOT_ERROR_NONE)
            continue;
        printf("Number of updates: %d\n", count);
        for (index = 0; index < count; index++)
        {
            message = updates[index].message;
            if (message.text)
            {
                    printf("%s: %s \n", message.from->first_name, message.text);
                    //telebot_send_dice(handle, message.chat->id, false, 0, "");
                    snprintf(message_reply, SIZE_OF_ARRAY(message_reply), "%s TEST", message.text);
                    if (strstr(convertToLowerCase(message.text), "/start"))
                    {
                        snprintf(message_reply, SIZE_OF_ARRAY(message_reply), "Hello %s", message.from->first_name);
                    }
                    error_status = telebot_send_message(handle, message.chat->id, message_reply, "HTML", false, false, updates[index].message.message_id, "");
                    if (error_status != TELEBOT_ERROR_NONE)
                    {
                        printf("Failed to send message: %d \n", error_status);

                    }
            }
            offset = updates[index].update_id + 1;
        }
        telebot_put_updates(updates, count);
        memset(message_reply,0, 4096);
        sleep(1);
    }

    telebot_destroy(handle);
}