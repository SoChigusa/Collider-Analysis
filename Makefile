CXX := icpc
CXXFLAGS := -g -Wall -std=c++11 -Ofast

all:		particle.o lhereader.o hepmc_reader.o

%.o:	%.cpp
		$(CXX) $(CXXFLAGS) -o $@ -c $<

test.out:	test.cpp
		$(CXX) $(CXXFLAGS) -o test.o particle.o lhereader.o hepmc_reader.o

clean:
		rm -rf *.o
