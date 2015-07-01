CC=g++
FLAGS=-std=c++11 -O2 -g

all: graph.o
	$(CC) $(FLAGS) graph.o -o graph.out
	#valgrind 
	./graph.out

graph.o: graph.cpp
	$(CC) $(FLAGS) -c graph.cpp -o graph.o

clean:
	rm *.o *.out

