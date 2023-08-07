#!/bin/bash
gcc -Wall database.c database.h finance.h stack.h -lsqlite3 -lm -o test && ./test
