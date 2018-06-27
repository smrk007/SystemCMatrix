#include <iostream>
#include "matrix2x2.cc"
#include <systemc.h>

int sc_main(int argc, char* argv[]) {

	// Declaring the signals
	sc_signal<int> test_input[2][2];

	// Connecting signals to the matrix
	matrix2x2 test("test");
	for (int row = 0; row < 2; row++) {
		for (int col = 0; col < 2; col++) {
			test.input_matrix[row][col](test_input[row][col]);
		}
	}

	// Initializing the matrix
	for (int row = 0; row < 2; row++) {
		for (int col = 0; col < 2; col++) {
			test_input[row][col].write(2*row + col + 1);
		}
	}
	test.initialize();

	// Testing the functionalities
	test.print();

	return 0;
}
