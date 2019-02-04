#John Rocco and Richard Zheng

all: compile link

compile:
	g++ -g -c -std=c++11 prog4swap.cpp -o prog4swap.o

link: compile
	g++ prog4swap.o -o prog4swap

clean:
	rm prog4swap.o prog4swap
