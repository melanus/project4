CC 	= g++
CFLAGS 	= -Wall -lpthread -lcurl

all: main

main: 
	$(CC) main.c -o main $(CFLAGS)

clean:
	rm ./main *.csv
