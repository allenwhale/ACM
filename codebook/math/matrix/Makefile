CC=g++
FLAGS=-std=c++11 -O2 -g

all: matrix.o
	$(CC) $(FLAGS) matrix.o -o matrix.out
	#valgrind 
	./matrix.out

matrix.o: matrix.cpp
	$(CC) $(FLAGS) -c matrix.cpp -o matrix.o

clean:
	rm *.o *.out
