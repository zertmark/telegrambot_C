BINARY_FILE = FinanceBot.bin
CC= gcc
CFLAGS = -O3 -I include -lcurl -ljson-c -lsqlite3 -lz -lm
OBJDIR
src_dir = ./src/
objects = bot.o database.o excel.o finance.o main.o out pretty_table.o stack.o token_reader.o
build_dir = $(addprefix $(OBJDIR)/,$(objects)) 

.PHONY: clean

$(BINARY_FILE): build

build: $(objects) # excel pretty_table bot	 
# build:
	# rm -f src/log.txt
	# sudo gcc -g -O3 -Iinclude src/* -lcurl -ljson-c -lsqlite3 -lz -lm -o $(BINARY_FILE)
# 
# run:
	# ./$(BINARY_FILE)
# 
# build_and_run: build run
# 
clean:
	rm -f src/log.txt
	rm -f log.txt
	rm -f ./$(BINARY_FILE)
