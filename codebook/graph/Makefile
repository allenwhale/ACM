CC=g++
FLAGS=-std=c++11 -O2 -g -Wall -Wextra

path: path.o
	$(CC) $(FLAGS) path.o -o path.out
	#valgrind 
	./path.out

path.o: path.cpp
	$(CC) $(FLAGS) -c path.cpp -o path.o

graph: graph.o
	$(CC) $(FLAGS) graph.o -o graph.out
	#valgrind 
	./graph.out

graph.o: graph.cpp
	$(CC) $(FLAGS) -c graph.cpp -o graph.o

clean:
	rm *.o *.out

