.PHONY: clean

server: server.c

client: client.c

clean:
	rm -rf server client
