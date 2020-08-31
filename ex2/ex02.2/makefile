CC = g++
CFLAGS = -Wall -O3 --std=c++11

main.exe : main.o random.o function.o vettore.o
	$(CC) random.o function.o vettore.o main.o -o main.exe
main.o : main.C
	$(CC) -c main.C -o main.o $(CFLAGS)
random.o : random.C random.h
	$(CC) -c random.C -o random.o $(CFLAGS)
function.o: function.C function.h
	$(CC) -c function.C -o function.o $(CFLAGS)
vettore.o : vettore.C vettore.h
	$(CC) -c vettore.C -o vettore.o $(CFLAGS)
clean :
	rm *.o main.exe seed.out
