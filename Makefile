a1q2: a1q2.cpp
	g++ -o a1q2 a1q2.cpp

a1q2old: a1q2-old.cpp
	g++ -o a1q2-old a1q2-old.cpp
	
a1q1: a1q1.cpp
	g++ -o a1q1 a1q1.cpp
	
test: test.cpp
	g++ -o  test test.cpp
	
rm: 
	rm *.o
