CXX = g++

LDLIBS = -lsystemc
LDFLAGS = -Wl,-stack_size -Wl,10000000
CXXFLAGS = -g

all:	  main.exe 

main.exe:	main.o
	$(CXX) -g -o $@ $< $(LDLIBS) -v $(LDFLAGS)

clean:
	rm -R *.o bin/*.exe *.vcd *.dSYM *.log *.sym
