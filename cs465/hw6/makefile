CC=gcc -Wall
LIB= -lssl -lcrypto
all:aes_encrypt
	./aes_encrypt
aes_encrypt: AES_encrypt.o
	$(CC) -o aes_encrypt aes_encrypt.o $(LIB)
AES_encrypt.o: AES_encrypt.c
	$(CC) -c AES_encrypt.c
clean:
	rm *.o
