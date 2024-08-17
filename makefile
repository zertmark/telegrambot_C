build:
	gcc --std=gnu18 -O3 src/* -lcurl -ljson-c -lsqlite3 -lz -lm -o FinanceBot.bin

run:
	./FinanceBot.bin

build_and_run:
	./compile.sh && ./FinanceBot.bin
clean:
	rm FinanceBot.bin