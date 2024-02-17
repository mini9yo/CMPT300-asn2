CFLAGS = -Wall -g -std=c99 -D _POSIX_C_SOURCE=200809L -Werror

all: build

build:
	gcc $(CFLAGS) list.c inputThread.c UDPThread.c printThread.c sendThread.c socket.c -o main -lpthread

run: build
	./main

valgrind: build
	valgrind --lead-check=full ./main

clean:
	rm -f main