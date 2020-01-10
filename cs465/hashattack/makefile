CC=gcc -Wall
LIB= -lssl -lcrypto
all:hash_attack
	./hashrun
hash_attack: hash.o
	$(CC) -o hashrun hash.o $(LIB)
hash.o: hash.c 
	$(CC) -c hash.c
clean:
	rm *.o