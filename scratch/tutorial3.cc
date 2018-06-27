#include <iostream>
#include <systemc.h>

SC_MODULE (test) {

	// Ports
	sc_in<int>	a[2][2];
	sc_out<int>	b[2][2];

	// Local Variables
	int data[2][2];

	void save_data() {
		for (int row = 0; row < 2; row++) {
			for (int col = 0; col < 2; col++) {
				data[row][col] = a[row][col].read();
			}
		}
	}

	void print() {
		for (int row = 0; row < 2; row++) {
			for (int col = 0; col < 2; col++) {
				std::cout << data[row][col] << " ";
			}
			std::cout << std::endl;
		}
	}

	SC_CTOR (test) {
		SC_METHOD (save_data);
		SC_METHOD (print);
	}
};

int sc_main(int argc, char* argv[]) {

	// Signals
	sc_signal<int>	a[2][2];
	sc_signal<int>	b[2][2];

	// Module
	test c("C");
	for (int row = 0; row < 2; row++) {
		for (int col = 0; col < 2; col++) {
			c.a[row][col](a[row][col]);
			c.b[row][col](b[row][col]);
		}
	}

	// Opening the VCD file
	sc_trace_file *wf = sc_create_vcd_trace_file("tester");
	for (int row = 0; row < 2; row++) {
		for (int col = 0; col < 2; col++) {
			sc_trace(wf, a[row][col], "a");
			sc_trace(wf, b[row][col], "b");
		}
	}

	// Assigning values
	for (int row = 0; row < 2; row++) {
		for (int col = 0; col < 2; col++) {
			a[row][col].write(2*row + col + 1);
		}
	}
	sc_start(1, SC_PS);
	c.save_data();
	sc_start(1, SC_PS);
	c.print();

	// Starting program

	return 0;
}
