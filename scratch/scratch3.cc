#include <iostream>
#include <systemc.h>

template <int A>
SC_MODULE (X) {

	// Processes
	void run () {
		for (int i = 0; i < A; i++) {
			std::cout << i << std::endl;
		}
	}

	SC_CTOR (X) {
		SC_METHOD (run);
	}
};

int sc_main(int argc, char* argv[]) {

	X<2> x("test");
	sc_start();


	return 0;
}
