#include <iostream>
#include <systemc.h>

SC_MODULE (adder) {
	sc_in<double> a, b;
	sc_out<double> c;

	void run() {
		std::cout << a + b;
	}

	SC_CTOR (adder) {
		// void
	}
};

int sc_main(int argc, char* argv[]) {

	adder four("four");
	

	return 0;
}
