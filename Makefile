CC=gcc
CFLAGS=-I.

all: client

client: client.c requests.c helpers.c buffer.c parson.c
	$(CC) -o client client.c requests.c helpers.c parson.c buffer.c -Wall

run: client
	./client

clean:
	rm -f *.o client
