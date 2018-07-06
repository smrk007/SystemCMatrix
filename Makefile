CXX = g++

LDLIBS = -lsystemc
CXXFLAGS = -g

all:	main.exe test1.exe

main.exe:	main.o SystemCMatrix.o ANN.h
	$(CXX) -g -o $@ $< $(LDLIBS) -v

test1.exe:	test1.o SystemCMatrix.o ANN.h
	$(CXX) -g -o $@ $< $(LDLIBS)

clean:
	rm *.o *.exe *.vcd
