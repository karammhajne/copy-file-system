CC = gcc
CFLAGS = -Wall -Wextra -std=c99

all: my_copy

my_copy: my_copy.c
	$(CC) $(CFLAGS) my_copy.c -o my_copy

clean:
	rm -f my_copy
