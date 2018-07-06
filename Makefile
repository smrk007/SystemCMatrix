CXX = g++

LDLIBS = -lsystemc
CXXFLAGS = -g

all:	bin/main.exe bin/test_sigmoid.exe bin/test_arr_mult.exe

bin/main.exe:	main.o SystemCMatrix.o ANN.h
	$(CXX) -g -o $@ $< $(LDLIBS) -v

bin/test_%.exe:	test_%.o SystemCMatrix.o ANN.h
	$(CXX) -g -o $@ $< $(LDLIBS)

clean:
	rm *.o bin/*.exe *.vcd
