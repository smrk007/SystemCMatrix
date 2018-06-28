CXX = g++

LDLIBS = -lsystemc

all:	main.exe

main.exe:	main.o SystemCMatrix.o ANN.o
	$(CXX) -o $@ main.o SystemCMatrix.o ANN.o $(LDLIBS)

clean:
	rm *.o *.exe *.vcd
