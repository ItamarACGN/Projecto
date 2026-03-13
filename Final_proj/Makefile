CC = gcc

#REMOVE the -g flag before we submit

CFLAGS = -ansi -Wall -Wextra -Werror -pedantic-errors
LDFLAGS = -lm

all: symnmf

symnmf: symnmf.o matrix.o
	$(CC) $(CFLAGS) -o symnmf symnmf.o matrix.o $(LDFLAGS)

symnmf.o: symnmf.c symnmf.h
	$(CC) $(CFLAGS) -c symnmf.c

matrix.o: matrix.c symnmf.h
	$(CC) $(CFLAGS) -c matrix.c

clean:
	rm -f *.o symnmf