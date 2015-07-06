CC=clang++
FLAGS=-std=c++11 -O2 -g -Wall -Wextra

geometry: geometry.o
	$(CC) $(FLAGS) geometry.o -o geometry.out
	#valgrind 
	./geometry.out

geometry.o: geometry.cpp
	$(CC) $(FLAGS) -c geometry.cpp -o geometry.o
