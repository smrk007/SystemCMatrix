#ifndef _MMLU_CC_
#define _MMLU_CC_

#include <systemc.h>

SC_MODULE (MMLU) {
// Matrix-Matrix Logic Unit

	// Ports
	sc_in<int>		input1[2][2];
	sc_in<int>		input2[2][2];
	sc_out<int>		output[2][2];

	// Operations
	void add() {
		for (int row = 0; row < 2; row++) {
			for (int col = 0; col < 2; col++) {
				output[row][col].write(input1[row][col].read() + input2[row][col].read());
			}
		}
	}

	void subtract() {
		for (int row = 0; row < 2; row++) {
			for (int col = 0; col < 2; col++) {
				output[row][col].write(input1[row][col].read() - input2[row][col].read());
			}
		}
	}

	SC_CTOR (MMLU) {
		SC_METHOD (add);
		SC_METHOD (subtract);
	}
};

#endif /* MMLU.cc */
