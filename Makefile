CXX := g++
CXXFLAGS := -g -Wall -std=c++11 -O2

all:		test.out

test.out:	test.cpp particle.h lhe_reader.h hepmc_reader.h
		$(CXX) $(CXXFLAGS) -o test.out test.cpp

clean:
		rm -rf *.o *.out
