a1q1: a1q1.c
	gcc -o a1q1 a1q1.c
	
test: test.cpp
	g++ -g -std=gnu++0x -o  test test.cpp
	
rm: 
	rm *.o
