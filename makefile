CC = gcc
CFLAGS = -Wall -Wextra -std=c99

all: copy_my

copy_my: c.copy_my
	$(CC) $(CFLAGS) copy_my.c -o copy_my

clean:
	rm -f copy_my
