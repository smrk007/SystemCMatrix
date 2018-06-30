#include <fstream>
#include <iomanip>
#include <iostream>
#include <systemc.h>

// #include "ANN.h"
#include "SystemCMatrix.h"

#define M1	2
#define S	2
#define M2	2	

int sc_main(int argc, char* argv[]) {

	// Signals
	sc_signal<int>		input1[M1][S];
	sc_signal<int>		input2[S][M2];
	sc_signal<int>		output[M1][M2];

	matrix_multiplier<M1,S,M2> mmult("MMULT");
	// Linking Signals to Ports
	for (int row = 0; row < M1; row++) {
			for (int col = 0; col < S; col++) {
				mmult.input1[row][col](input1[row][col]);
			}
	}
	for (int row = 0; row < S; row++) {
			for (int col = 0; col < M2; col++) {
				mmult.input2[row][col](input2[row][col]);
			}
	}
	for (int row = 0; row < M1; row++) {
			for (int col = 0; col < M2; col++) {
				mmult.output[row][col](output[row][col]);
			}
	}

	// Putting signals into trace file
	sc_trace_file *trace_file = sc_create_vcd_trace_file("TEST");
	for (int row = 0; row < M1; row++) {
			for (int col = 0; col < S; col++) {
				sc_trace(trace_file, input1[row][col], "input1");
			}
	}
	for (int row = 0; row < S; row++) {
			for (int col = 0; col < M2; col++) {
				sc_trace(trace_file, input2[row][col], "input2");
			}
	}
	for (int row = 0; row < M1; row++) {
			for (int col = 0; col < M2; col++) {
				sc_trace(trace_file, output[row][col], "output");
			}
	}

	// Sending data to signals
	sc_start(1, SC_NS);
	for (int row = 0; row < M1; row++) {
			for (int col = 0; col < S; col++) {
				input1[row][col] = 2*row + col + 1;
			}
	}
	for (int row = 0; row < S; row++) {
			for (int col = 0; col < M2; col++) {
				input2[row][col] = 2*row + col + 1;
			}
	}
	sc_start(1, SC_NS);

	for (int row = 0; row < M1; row++) {
			for (int col = 0; col < M2; col++) {
				std::cout << std::setw(5) << output[row][col] << " ";
			}
			std::cout << std::endl;
	}

	return 0;
}
