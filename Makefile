CXX = g++

LDLIBS = -lsystemc
CXXFLAGS = -g

all:	main.exe

main.exe:	main.o SystemCMatrix.o ANN.o
	$(CXX) -g -o $@ main.o SystemCMatrix.o ANN.o $(LDLIBS)

main.o:	main.cc 

clean:
	rm *.o *.exe *.vcd
