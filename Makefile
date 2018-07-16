CXX = g++

LDLIBS = -lsystemc
LDFLAGS = -Wl,-stack_size -Wl,1000000000
CXXFLAGS = -g

all:	bin/main.exe bin/test_sigmoid.exe bin/test_arr_mult.exe

bin/main.exe:	main.o SystemCMatrix.o ANN.h
	$(CXX) -g -o $@ $< $(LDLIBS) -v $(LDFLAGS)

bin/test_%.exe:	test_%.o SystemCMatrix.o ANN.h
	$(CXX) -g -o $@ $< $(LDLIBS)

clean:
	rm *.o bin/*.exe *.vcd
