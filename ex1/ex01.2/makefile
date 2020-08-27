CC = g++
CFLAGS = -Wall -O3 --std=c++11

main.exe : main.o random.o 
	$(CC) random.o main.o -o main.exe
main.o : main.C
	$(CC) -c main.C -o main.o $(CFLAGS)
random.o : random.C random.h
	$(CC) -c random.C -o random.o $(CFLAGS)
clean :
	rm *.o main.exe seed.out
