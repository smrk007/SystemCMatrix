
CXX = g++
LDLIBS = -lsystemc

all:	scratch1.exe tutorial1.exe tutorial2.exe

scratch1.exe:	scratch1.o
	$(CXX) -o $@ $< $(LDLIBS)

tutorial1.exe:	tutorial1.o
	$(CXX) -o $@ $< $(LDLIBS)

tutorial2.exe:	tutorial2.o
	$(CXX) -o $@ $< $(LDLIBS)

clean:
	rm *.exe *.o
