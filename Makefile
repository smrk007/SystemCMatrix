CXX = g++

LDLIBS = -lsystemc

all:	main.exe

main.exe:	main.o
	$(CXX) -o $@ $< $(LDLIBS)

clean:
	rm *.o *.exe *.vcd
