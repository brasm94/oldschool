CC=gcc -W -Wall -fmax-errors=5
LIB= -lssl -lcrypto
all:mod_exp
	./me
mod_exp:mod.o
	$(CC) -o me mod.c $(LIB)
clean:
	rm *.o
