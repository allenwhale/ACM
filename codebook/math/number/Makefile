CC=g++
FLAGS=-std=c++11 -O2 -g

all: number.o
	$(CC) $(FLAGS) number.o -o number.out
	#valgrind 
	./number.out

number.o: number.cpp
	$(CC) $(FLAGS) -c number.cpp -o number.o

clean:
	rm *.o *.out

