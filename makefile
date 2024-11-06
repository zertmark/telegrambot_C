BINARY_FILE = FinanceBot.bin
# CC= gcc
# CFLAGS = -O3 -I include -lcurl -ljson-c -lsqlite3 -lz -lm
# src_dir = src/
# objects = build/token_reader.o build/main.o build/bot.o  #database.o excel.o finance.o  out pretty_table.o stack.o
# build_dir = $(addprefix $(OBJDIR)/,$(objects))

.PHONY: clean
# 
# $(BINARY_FILE): $(objects)
	# $(CC) $(CFLAGS) $^ -o test
# 
# build/token_reader.o: src/token_reader.c src/token_reader.h
# build/main.o: src/main.c src/bot.h src/token_reader.h
# build/bot.o: src/bot.c src/bot.h src/commands.h include/telebot-common.h
# 
# 
# src/main.c:
# $(CC) -c -O3 src/main.c -o build/main.o
# 
# src/bot.c:
# $(CC) -c -O3 src/libtelebot_static.a -Iinclude src/bot.c -o build/bot.o 
# src/token_reader.c:
	# $(CC) -c -O3 src/token_reader.c -o build/token_reader.o
build: build_d
	echo "Project is build!\nFile:$(BINARY_FILE)"

build_d:
	$(clean)
	sudo gcc -g -O3 -Iinclude src/* -lcurl -ljson-c -lsqlite3 -lz -lm -o $(BINARY_FILE)

build_r:
	$(clean)
	sudo gcc -O3 -Iinclude src/* -lcurl -ljson-c -lsqlite3 -lz -lm -o $(BINARY_FILE)
	
run:
	./$(BINARY_FILE)

build_and_run: build_d run

clean:
	rm -f src/log.txt
	rm -f log.txt
	rm -f ./$(BINARY_FILE)
