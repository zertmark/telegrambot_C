#!/bin/bash
gcc --std=gnu18 -O3 src/* -lcurl -ljson-c -lsqlite3 -lz -lm -o FinanceBot.bin
