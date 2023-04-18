CC = gcc
CFLAGS = -Wall -Wextra -Werror -ansi -pedantic -o proj2 *.c
EXECUTABLE = projeto

all: project1.c
	$(CC) $(CFLAGS)

clean:
	rm -f *.o $(EXECUTABLE)