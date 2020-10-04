#
# example Makefile to build an executable named myprog from myprog.cpp
#

all: sieve_of_ero.cpp matrix_mult.cpp Game_Of_Life.c
	g++ -std=c++11 -Wall -fopenmp sieve_of_ero.cpp -o sieve_of_ero
	g++ -std=c++11 -Wall -fopenmp matrix_mult.cpp -o matrix_mult
	gcc -fopenmp -Wall Game_Of_Life.c -o Game_Of_Life 

clean:
	rm sieve_of_ero
	rm matrix_mult
	rm Game_Of_Life
