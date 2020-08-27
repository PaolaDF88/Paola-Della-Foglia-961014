CC = g++
CFLAGS = -Wall -O3 --std=c++11

main.exe : main.o random.o vettore.o statistiche.o 
	$(CC) random.o vettore.o statistiche.o main.o -o main.exe
main.o : main.C
	$(CC) -c main.C -o main.o $(CFLAGS)
random.o : random.C random.h
	$(CC) -c random.C -o random.o $(CFLAGS)
statistiche.o : statistiche.C statistiche.h
	$(CC) -c statistiche.C -o statistiche.o $(CFLAGS)
vettore.o : vettore.C vettore.h
	$(CC) -c vettore.C -o vettore.o $(CFLAGS)
clean :
	rm *.o main.exe seed.out
