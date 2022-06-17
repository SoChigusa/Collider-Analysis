CXX := g++
CXXFLAGS := -g -Wall -std=c++11 -O2

all:		test.out

test.out:	test.cpp
		$(CXX) $(CXXFLAGS) -o test.out test.cpp

clean:
		rm -rf *.o
