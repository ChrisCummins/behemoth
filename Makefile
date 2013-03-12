CC := gcc
CFLAGS := -Wall -Wextra -Wno-unused-parameter

all: server client

server: server.o
	$(CC) -o $@ $^

client: client.o
	$(CC) -o $@ $^

%.o: %.c
	$(CC) -c $< $(CFLAGS)

clean:
	rm -f server.o server
