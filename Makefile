CXX = g++

LDLIBS = -lsystemc
LDFLAGS = -Wl,-stack_size -Wl,10000000
CXXFLAGS = -g

all:	  bin/main.exe 

bin/main.exe:	main.o SystemCMatrix.o
	$(CXX) -g -o $@ $< $(LDLIBS) -v $(LDFLAGS)

clean:
	rm -R *.o bin/*.exe *.vcd *.dSYM *.log *.sym
