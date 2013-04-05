.PHONY: clean

CFLAGS=

test: server

build: server

test: CFLAGS=-g -DLOG_LEVEL=DEBUG

build: CFLAGS=-DLOG_LEVEL=NORMAL

server: dev.o http_header.o logging.o http.o request.o response.o server.c

http.o: error.o

client: client.c

clean:
	rm -rf server client
	rm -rf *.o
