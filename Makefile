CC = gcc
CFLAGS = -Wall -Wextra

process: process.c
	$(CC) $(CFLAGS) process.c -o process

clean:
	rm -f process
