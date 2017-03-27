CC 	= g++
CFLAGS 	= -Wall -lpthread -lcurl -std=c++11

all: main

main: classes.cpp libcurl.cpp main.cpp
	$(CC) main.cpp -o main $(CFLAGS)

clean:
	rm ./main *.csv
