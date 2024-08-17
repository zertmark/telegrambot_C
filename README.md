# README
This is Telegram bot that can manage products and finances\
Add your token in main.c
#Installation
You need to install json-c
# Compile
gcc --std=gnu18 -O3 src/* -lcurl -ljson-c -lsqlite3 -lz -lm -o FinanceBot.bin
### OR
./compile.sh
# TO:DO
1. Fix text align on some commands
2. Refactor most of the code because right now it looks awful
3. Add normal table for user to know how to write commands to bot
4. (Optional) Make binary file smaller than 1.44 MB
