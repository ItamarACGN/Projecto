CC = gcc

#REMOVE the -g flag before we submit

CFLAGS = -ansi -Wall -Wextra -Werror -pedantic-errors -g
LDFLAGS = -lm

all: symnmf symnmfmodule

symnmf: symnmf.o matrix.o
	$(CC) $(CFLAGS) -o symnmf symnmf.o matrix.o $(LDFLAGS)

symnmf.o: symnmf.c symnmf.h
	$(CC) $(CFLAGS) -c symnmf.c

matrix.o: matrix.c symnmf.h
	$(CC) $(CFLAGS) -c matrix.c

symnmfmodule: symnmfmodule.o symnmf.o matrix.o
	$(CC) $(CFLAGS) -shared -o symnmfmodule.so symnmfmodule.o symnmf.o matrix.o $(LDFLAGS)

symnmfmodule.o: symnmfmodule.c symnmf.h
	$(CC) $(CFLAGS) -fPIC -c symnmfmodule.c

clean:
	rm -f *.o symnmf *.so