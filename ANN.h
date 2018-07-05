#ifndef _ANN_H_
#define _ANN_H_

#include <iostream>
#include <systemc.h>
#include "SystemCMatrix.h"

template <int BATCH_SIZE, int INPUT_DIMENSION, int HIDDEN_LAYER_DIMENSION, int OUTPUT_DIMENSION>
SC_MODULE (feed_forward) {

	// Ports
	sc_in<int>		input[BATCH_SIZE][INPUT_DIMENSION];
	sc_in<int>		weights1[INPUT_DIMENSION][HIDDEN_LAYER_DIMENSION];
	sc_in<int>		weights2[HIDDEN_LAYER_DIMENSION][OUTPUT_DIMENSION];

	sc_out<int>		out1[BATCH_SIZE][INPUT_DIMENSION];
	sc_out<int>		out2[BATCH_SIZE][HIDDEN_LAYER_DIMENSION];
	sc_out<int>		outf[BATCH_SIZE][OUTPUT_DIMENSION];

	// Signals
	sc_signal<int>	mult_out1[BATCH_SIZE][HIDDEN_LAYER_DIMENSION];
	sc_signal<int>	mult_out2[BATCH_SIZE][HIDDEN_LAYER_DIMENSION];

	// Sub-Modules
	matrix_multiplier<BATCH_SIZE,INPUT_DIMENSION,HIDDEN_LAYER_DIMENSION>	mmult1;
	matrix_multiplier<BATCH_SIZE,HIDDEN_LAYER_DIMENSION,OUTPUT_DIMENSION>	mmult2;

	// Process
	void set_out1() {
		for (int row = 0; row < BATCH_SIZE; row++) {
			for (int col = 0; col < INPUT_DIMENSION; col++) {
					out1[row][col].write(input[row][col].read());
				}
		}
		std::cout << "Output1" << std::endl;
	}

	void set_out2() {
		for (int row = 0; row < BATCH_SIZE; row++) {
				for (int col = 0; col < HIDDEN_LAYER_DIMENSION; col++) {
					out2[row][col].write(mult_out1[row][col].read());
				}
		}
		std::cout << "Output2" << std::endl;
	}

	void set_outf() {
		for (int row = 0; row < BATCH_SIZE; row++) {
				for (int col = 0; col < OUTPUT_DIMENSION; col++) {
					outf[row][col].write(mult_out2[row][col].read());
				}
		}
		std::cout << "Output3" << std::endl;
	}

	// Signals
	SC_CTOR (feed_forward) : mmult1("MMULT1"), mmult2("MMULT2") {
		// Defining output behaviors
		SC_METHOD (set_out1);
			dont_initialize();
			for (int row = 0; row < BATCH_SIZE; row++) {
				for (int col = 0; col < INPUT_DIMENSION; col++) {
					sensitive << input[row][col];
				}
			}
		SC_METHOD (set_out2);
			dont_initialize();
			for (int row = 0; row < BATCH_SIZE; row++) {
				for (int col = 0; col < HIDDEN_LAYER_DIMENSION; col++) {
					sensitive << mult_out1[row][col];
				}
			}
		SC_METHOD (set_outf);
			dont_initialize();
			for (int row = 0; row < BATCH_SIZE; row++) {
				for (int col = 0; col < OUTPUT_DIMENSION; col++) {
					sensitive << mult_out2[row][col];
				}
			}
		// Connecting Modules
		// mmult1
		for (int row = 0; row < BATCH_SIZE; row++) {
			for (int col = 0; col < INPUT_DIMENSION; col++) {
				mmult1.input1[row][col](input[row][col]);
			}
		}
		for (int row = 0; row < INPUT_DIMENSION; row++) {
			for (int col = 0; col < HIDDEN_LAYER_DIMENSION; col++) {
				mmult1.input2[row][col](weights1[row][col]);
			}
		}
		for (int row = 0; row < BATCH_SIZE; row++) {
			for (int col = 0; col < HIDDEN_LAYER_DIMENSION; col++) {
				mmult1.output[row][col](mult_out1[row][col]);
			}
		}
		// mmult2
		for (int row = 0; row < BATCH_SIZE; row++) {
			for (int col = 0; col < HIDDEN_LAYER_DIMENSION; col++) {
				mmult2.input1[row][col](mult_out1[row][col]);
			}
		}
		for (int row = 0; row < HIDDEN_LAYER_DIMENSION; row++) {
			for (int col = 0; col < OUTPUT_DIMENSION; col++) {
				mmult2.input2[row][col](weights2[row][col]);
			}
		}
		for (int row = 0; row < BATCH_SIZE; row++) {
			for (int col = 0; col < OUTPUT_DIMENSION; col++) {
				mmult2.output[row][col](mult_out2[row][col]);
			}
		}
	}
};

#endif /* ANN.h */
