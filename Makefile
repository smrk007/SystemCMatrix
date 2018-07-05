CXX = g++

LDLIBS = -lsystemc
CXXFLAGS = -g

all:	main.exe

main.exe:	main.o SystemCMatrix.o ANN.o
	$(CXX) -g -o $@ $< $(LDLIBS)

test1.exe:	test1.o SystemCMatrix.o ANN.o
	$(CXX) -g -o $@ $< $(LDLIBS)

main.o:	main.cc 

clean:
	rm *.o *.exe *.vcd
