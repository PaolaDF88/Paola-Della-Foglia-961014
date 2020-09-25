CC = g++
CFLAGS = -Wall -O3 --std=c++11

main.exe : main.o random.o vettore.o funzioni.o
	$(CC) funzioni.o vettore.o random.o main.o -o main.exe
main.o : main.C
	$(CC) -c main.C -o main.o $(CFLAGS)
random.o : random.C random.h
	$(CC) -c random.C -o random.o $(CFLAGS)
vettore.o : vettore.C vettore.h
	$(CC) -c vettore.C -o vettore.o $(CFLAGS)
funzioni.o : funzioni.C funzioni.h 
	$(CC) -c funzioni.C -o funzioni.o  $(CFLAGS)
clean :
	rm *.o main.exe seed.out
