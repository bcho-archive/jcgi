.PHONY: clean

CFLAGS=-g

server: request.o server.c

client: client.c

clean:
	rm -rf server client
