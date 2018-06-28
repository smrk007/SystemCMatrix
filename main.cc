#include <fstream>
#include <iostream>
#include <systemc.h>

#include "ANN.h"
#include "SystemCMatrix.h"

int sc_main(int argc, char* argv[]) {

	// Signals
	sc_signal<int>		input[2][2];
	sc_signal<int>		weights1[2][2];
	sc_signal<int>		weights2[2][2];

	sc_signal<int>		out1[2][2];
	sc_signal<int>		out2[2][2];
	sc_signal<int>		outf[2][2];

	// Connecting Signals to Ports
	feed_forward basic_ann("basic_ann");
	for (int row = 0; row < 2; row++) {
			for (int col = 0; col < 2; col++) {
				basic_ann.input[row][col](input[row][col]);
				basic_ann.weights1[row][col](weights1[row][col]);
				basic_ann.weights2[row][col](weights2[row][col]);
				basic_ann.out1[row][col](out1[row][col]);
				basic_ann.out2[row][col](out2[row][col]);
				basic_ann.outf[row][col](outf[row][col]);
			}
	}
	sc_trace_file *trace_file = sc_create_vcd_trace_file("ANN");
	for (int row = 0; row < 2; row++) {
			for (int col = 0; col < 2; col++) {
				sc_trace(trace_file, input[row][col], "input");
				sc_trace(trace_file, weights1[row][col], "weights1");
				sc_trace(trace_file, weights2[row][col], "weights2");
				sc_trace(trace_file, out1[row][col], "out1");
				sc_trace(trace_file, out2[row][col], "out2");
				sc_trace(trace_file, outf[row][col], "outf");
			}
	}

	// Linking Data into Ports
	input[0][0] = 1;
	input[0][1] = 2;
	input[1][0] = 3;
	input[1][1] = 4;

	weights1[0][0] = 3;
	weights1[0][1] = 1;
	weights1[1][0] = 2;
	weights1[1][1] = 4;

	weights2[0][0] = 2;
	weights2[0][1] = 4;
	weights2[1][0] = 3;
	weights2[1][1] = 1;

	sc_start(1, SC_NS);

	for (int row = 0; row < 2; row++) {
			for (int col = 0; col < 2; col++) {
				std::cout << outf[row][col] << " ";
			}
			std::cout << std::endl;
	}

	return 0;
}
