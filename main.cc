#include <fstream>
#include <iostream>
#include <systemc.h>

// #include "ANN.h"
#include "SystemCMatrix.h"

#define M1 2
#define S 2
#define M2 2

int sc_main(int argc, char* argv[]) {

	// Signals
	sc_signal<int>		input1[2][2];
	sc_signal<int>		input2[2][2];
	sc_signal<int>		output[2][2];

	matrix_multiplier<M1,S,M2> mmult("MMULT");
	for (int row = 0; row < 2; row++) {
			for (int col = 0; col < 2; col++) {
				mmult.input1[row][col](input1[row][col]);
				mmult.input2[row][col](input2[row][col]);
				mmult.output[row][col](output[row][col]);
			}
	}
	sc_trace_file *trace_file = sc_create_vcd_trace_file("TEST");
	for (int row = 0; row < 2; row++) {
			for (int col = 0; col < 2; col++) {
				sc_trace(trace_file, input1[row][col], "input1");
				sc_trace(trace_file, input2[row][col], "input2");
				sc_trace(trace_file, output[row][col], "output");
			}
	}

	sc_start(1, SC_NS);
	for (int row = 0; row < 2; row++) {
			for (int col = 0; col < 2; col++) {
				input1[row][col] = 2*row + col + 1;
				input2[row][col] = 2*row + col + 1;
			}
	}
	sc_start(1, SC_NS);

	for (int row = 0; row < 2; row++) {
			for (int col = 0; col < 2; col++) {
				std::cout << output[row][col] << std::endl;
			}
	}

	return 0;
}
