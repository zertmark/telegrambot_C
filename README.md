# README
This is Telegram bot that can manage products and finances\
Add your token in .token file
#Installation
You need to install json-c curl and sqlite3 (all C libriaries)
# Compile
> gcc --std=gnu18 -O3 src/* -lcurl -ljson-c -lsqlite3 -lz -lm -o FinanceBot.bin
### OR
> make
# TO:DO
1. Remake makefile
2. Fix text align on some commands
3. Refactor most of the code because right now it looks awful
4. Add normal table for user to know how to write commands to bot
5. (Optional) Make binary file smaller than 1.44 MB