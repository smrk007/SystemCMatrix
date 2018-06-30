#ifndef _ANN_H_
#define _ANN_H_

#include <iostream>
#include <systemc.h>
#include "SystemCMatrix.h"

SC_MODULE (feed_forward) {

	// Ports
	sc_in<int>		input[2][2];
	sc_in<int>		weights1[2][2];
	sc_in<int>		weights2[2][2];

	sc_out<int>		out1[2][2];
	sc_out<int>		out2[2][2];
	sc_out<int>		outf[2][2];

	// Signals
	sc_signal<int>	mult_out1[2][2];
	sc_signal<int>	mult_out2[2][2];

	// Sub-Modules
	matrix_multiplier<2,2,2>	mmult1, mmult2;

	// Process
	void set_out1();
	void set_out2();
	void set_outf();

	// Signals
	SC_CTOR (feed_forward) : mmult1("MMULT1"), mmult2("MMULT2") {
		// Defining output behaviors
		SC_METHOD (set_out1);
			dont_initialize();
			for (int row = 0; row < 2; row++) {
				for (int col = 0; col < 2; col++) {
					sensitive << input[row][col];
				}
			}
		SC_METHOD (set_out2);
			dont_initialize();
			for (int row = 0; row < 2; row++) {
				for (int col = 0; col < 2; col++) {
					sensitive << mult_out1[row][col];
				}
			}
		SC_METHOD (set_outf);
			dont_initialize();
			for (int row = 0; row < 2; row++) {
				for (int col = 0; col < 2; col++) {
					sensitive << mult_out2[row][col];
				}
			}
		// Connecting Modules
		for (int row = 0; row < 2; row++) {
			for (int col = 0; col < 2; col++) {
				// mmult1
				mmult1.input1[row][col](input[row][col]);
				mmult1.input2[row][col](weights1[row][col]);
				mmult1.output[row][col](mult_out1[row][col]);
				// mmult2
				mmult2.input1[row][col](mult_out1[row][col]);
				mmult2.input2[row][col](weights2[row][col]);
				mmult2.output[row][col](mult_out2[row][col]);
			}
		}
	}
};

#endif /* ANN.h */
