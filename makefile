CC 	= g++
CFLAGS 	= -Wall -lpthread -lcurl -std=c++11

all: main

main: 
	$(CC) main.cpp -o main $(CFLAGS)

clean:
	rm ./main *.csv
