#ifndef _SYSTEMCMATRIX_H_
#define _SYSTEMCMATRIX_H_

#include <systemc.h>

SC_MODULE (matrix_adder) {
	// Matrix-Matrix Logic Unit

	// Ports
	sc_in<int>		input1[2][2];
	sc_in<int>		input2[2][2];
	sc_out<int>		output[2][2];

	// Operation
	void add();

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
	void multiply();
	void clear_temp();

	SC_CTOR (matrix_multiplier) {
		SC_METHOD (multiply);
		SC_METHOD (clear_temp);
	}
};

#endif /* SystemCMatrix.h */
