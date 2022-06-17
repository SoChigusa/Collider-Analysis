CXX := g++
CXXFLAGS := -g -Wall -std=c++11 -O3

all:		particle.o lhe_reader.o hepmc_reader.o

%.o:	%.cpp %.h
		$(CXX) $(CXXFLAGS) -o $@ -c $<

test.out:	test.cpp
		$(CXX) $(CXXFLAGS) -o test.o particle.o lhe_reader.o hepmc_reader.o

clean:
		rm -rf *.o
