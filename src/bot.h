#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <unistd.h>
#include <telebot.h>
#include <telebot-core.h>
#include <telebot-types.h>
#include <ctype.h>
int tryTocreateBot(char *token);
char* getHelp();
void startBot(char *token);