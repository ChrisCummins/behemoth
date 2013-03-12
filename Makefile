CC := gcc
CFLAGS := -Wall -Wextra -Wno-unused-parameter

OBJ := $(wildcard *.o)

all: server client

server: server.o common.o
	$(CC) -o $@ $^

client: client.o common.o
	$(CC) -o $@ $^

%.o: %.c
	$(CC) -c $< $(CFLAGS)

clean:
	rm -f $(OBJ) server client
