CC=gcc -W -Wall -fmax-errors=5
# LIB= -lssl -lcrypto
all:mac_attack
	./sha1
mac_attack: sha1.o
	$(CC) -o sha1 sha1.o
sha1.o: sha1.c  sha1.h test.c
	$(CC) -c sha1.c sha1.h test.c
clean:
	rm *.o