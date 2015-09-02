CC=g++
FLAGS=-std=c++11 -O2 -g -Wall -Wextra

splay: splay.o
	$(CC) $(FLAGS) splay.o -o splay.out
	#valgrind 
	./splay.out

splay.o: splay.cpp
	$(CC) $(FLAGS) -c splay.cpp -o splay.o

treap: treap.o
	$(CC) $(FLAGS) treap.o -o treap.out
	#valgrind 
	./treap.out

treap.o: treap.cpp
	$(CC) $(FLAGS) -c treap.cpp -o treap.o
