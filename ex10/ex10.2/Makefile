CC = g++
CFLAGS = -Wall -O3 --std=c++11

main.exe : main.cpp main.h random.o city.o percorso.o
	mpic++ random.o main.cpp city.o percorso.o -o main.exe
random.o : random.cpp random.h
	$(CC) -c random.cpp -o random.o $(CFLAGS)
city.o : city.cpp city.h
	$(CC) -c city.cpp -o city.o $(CFLAGS)
percorso.o : percorso.cpp percorso.h
	$(CC) -c percorso.cpp -o percorso.o $(CFLAGS)
clean :
	rm *.o main.exe
