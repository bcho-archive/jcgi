.PHONY: clean

CFLAGS=-g

server: dev.o logging.o http.o request.o response.o server.c

http.o: error.o

client: client.c

clean:
	rm -rf server client
	rm -rf *.o
