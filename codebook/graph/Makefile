CC=g++
FLAGS=-std=c++11 -O2 -g -Wall -Wextra

tree: tree.o
	$(CC) $(FLAGS) tree.o -o tree.out
	#valgrind 
	./tree.out

tree.o: tree.cpp
	$(CC) $(FLAGS) -c tree.cpp -o tree.o

match: match.o
	$(CC) $(FLAGS) match.o -o match.out
	#valgrind 
	./match.out

match.o: match.cpp
	$(CC) $(FLAGS) -c match.cpp -o match.o

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

