#ifndef _SYSTEMCMATRIX_CC_
#define _SYSTEMCMATRIX_CC_

#include <systemc.h>

SC_MODULE (matrix_adder) {
// Matrix-Matrix Logic Unit

	// Ports
	sc_in<int>		input1[2][2];
	sc_in<int>		input2[2][2];
	sc_out<int>		output[2][2];

	// Operation
	void add() {
		for (int row = 0; row < 2; row++) {
			for (int col = 0; col < 2; col++) {
				output[row][col].write(input1[row][col].read() + input2[row][col].read());
			}
		}
	}

	SC_CTOR (matrix_adder) {
		SC_METHOD (add);
	}
};

SC_MODULE (matrix_multiplier) {

	// Ports
	sc_in<int>		input1[2][2];
	sc_in<int>		input2[2][2];
	sc_out<int>		output[2][2];

	// Variables
	int				temp[2][2];

	// Operation
	void multiply() {
		clear_temp();
		// Filling Temp
		for (int i = 0; i < 2; i++) {
		for (int j = 0; j < 2; j++) {
		for (int k = 0; k < 2; k++) {
			temp[i][j] += input1[i][k].read() * input2[k][j].read();
		}}}
		// Sending Temp to Output
		for (int row = 0; row < 2; row++) {
			for (int col = 0; col < 2; col++) {
				output[row][col].write(temp[row][col]);
			}
		}
	}

	void clear_temp() {
		for (int row = 0; row < 2; row++) {
			for (int col = 0; col < 2; col++) {
				temp[row][col] = 0;
			}
		}
	}

	SC_CTOR (matrix_multiplier) {
		SC_METHOD (multiply);
		SC_METHOD (clear_temp);
	}
};

#endif /* SystemCMatrix.cc */
