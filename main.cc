#include <fstream>
#include <iostream>
#include <systemc.h>

#include "SystemCMatrix.cc"

int sc_main(int argc, char* argv[]) {

	// Signals
	sc_signal<int>		matrix1[2][2];
	sc_signal<int>		matrix2[2][2];
	sc_signal<int>		matrix_out[2][2];

	// Linking Signals to Ports
	matrix_multiplier adder("MMLU");
	for (int row = 0; row < 2; row++) {
			for (int col = 0; col < 2; col++) {
				adder.input1[row][col](matrix1[row][col]);
				adder.input2[row][col](matrix2[row][col]);
				adder.output[row][col](matrix_out[row][col]);
			}
	}
	sc_trace_file *trace_file = sc_create_vcd_trace_file("Matrix_Addition");
	for (int row = 0; row < 2; row++) {
			for (int col = 0; col < 2; col++) {
				sc_trace(trace_file, matrix1[row][col], "matrix1");
				sc_trace(trace_file, matrix2[row][col], "matrix2");
				sc_trace(trace_file, matrix_out[row][col], "matrix_out");
			}
	}
		
	// Sending values to the input ports
	for (int row = 0; row < 2; row++) {
			for (int col = 0; col < 2; col++) {
				matrix1[row][col] = 2*row + col + 1;
				matrix2[row][col] = 2*row + col + 1;
			}
	}

	sc_start(1, SC_NS);
	adder.multiply();
	sc_start(1, SC_NS);

	// Debugging
	for (int row = 0; row < 2; row++) {
			for (int col = 0; col < 2; col++) {
				std::cout << matrix_out[row][col].read() << " ";
			}
			std::cout << std::endl;
	}

	return 0;
}
