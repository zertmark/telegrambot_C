BINARY_FILE = FinanceBot.bin

build:
	rm -f src/log.txt
	sudo gcc -g -O3 -Iinclude src/* -lcurl -ljson-c -lsqlite3 -lz -lm -o $(BINARY_FILE)

run:
	./$(BINARY_FILE)

build_and_run: build run

clean:
	rm -f src/log.txt
	rm -f log.txt
	rm -f ./$(BINARY_FILE)
