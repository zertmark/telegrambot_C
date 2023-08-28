#!/bin/bash
gcc -Wno-implicit -Wno-builtin-declaration-mismatch database.c database.h finance.h stack.h excel.h libxlsxwriter.a -lsqlite3 -lm -lz -o test
./test
