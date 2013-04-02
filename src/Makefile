.PHONY: clean

CFLAGS=-g

server: http.o request.o server.c

http.o: error.o

client: client.c

clean:
	rm -rf server client
	rm -rf *.o
