#!/bin/bash
gcc --std=gnu18 -g src/* -lcurl -ljson-c -lsqlite3 -lz -lm -o FinanceBot.bin
