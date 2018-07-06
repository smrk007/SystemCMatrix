#include <iostream>
#include <systemc.h>

#include "SystemCMatrix.h"

#define D1 1
#define D2 10

int sc_main(int argc, char* argvp[]) {

	// Signals
	sc_signal<float>	input[D1][D2];
	sc_signal<float>	output[D1][D2];

	// Initialization
	sigmoid_activation<D1,D2> sig_test("SIG_TEST");
	for (int row = 0; row < D1; row++) {
			for (int col = 0; col < D2; col++) {
				sig_test.input[row][col](input[row][col]);
				sig_test.output[row][col](output[row][col]);
			}
	}

	sc_trace_file *trace_file = sc_create_vcd_trace_file("TEST");
	for (int row = 0; row < D1; row++) {
			for (int col = 0; col < D2; col++) {
				sc_trace(trace_file, input[row][col], "input");
				sc_trace(trace_file, output[row][col], "output");
			}
	}
	sc_start(1, SC_NS);

	// Loading Data
	for (int row = 0; row < D1; row++) {
			for (int col = 0; col < D2; col++) {
				input[row][col] = 0.5*col - 1.5;
			}
	}
	sc_start(1, SC_NS);

	// Debugging
	for (int row = 0; row < D1; row++) {
			for (int col = 0; col < D2; col++) {
				std::cout << output[row][col] << " ";
			}
		std::cout << std::endl;
	}

    return 0;
}