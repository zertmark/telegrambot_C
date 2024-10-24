#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <unistd.h>
#include "telebot.h"
#include <ctype.h>

int tryTocreateBot(char *token);


char* getHelp();
void startBot();
void stopBot();