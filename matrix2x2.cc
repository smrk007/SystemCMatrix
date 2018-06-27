#include <systemc.h>

SC_MODULE (matrix2x2) {

	// Signals
	sc_in<int> i1, i2, i3, i4;
	sc_out<int> o1, o2, o3, o4;

	// Local Data
	int data[2][2];

	// Functions
	void initialize() {
		std::cout << i1.read() << std::endl;
	}

	void print() {

	}

	SC_CTOR (matrix2x2) {
		SC_METHOD (initialize);
		SC_METHOD (print);
	}

};
