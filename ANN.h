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

template <int BATCH_SIZE, int OUTPUT_DIMENSION>
// NOTE: This current implementation is based on the assumption that there will only be one hidden layer
SC_MODULE (calculate_final_delta) {

	// Ports
	sc_in<float>		labels[BATCH_SIZE][OUTPUT_DIMENSION];
	sc_in<float>		outf[BATCH_SIZE][OUTPUT_DIMENSION];
	sc_out<float>		output[BATCH_SIZE][OUTPUT_DIMENSION];

	// Signals
	sc_signal<float>	subtract1_out[BATCH_SIZE][OUTPUT_DIMENSION];
	sc_signal<float>	arr_mult1_out[BATCH_SIZE][OUTPUT_DIMENSION];
	sc_signal<float>	ones_output[BATCH_SIZE][OUTPUT_DIMENSION];
	sc_signal<float>	subtract2_out[BATCH_SIZE][OUTPUT_DIMENSION];

	// Sub-Modules
	array_subtractor<BATCH_SIZE,OUTPUT_DIMENSION>	subtract1;
	array_multiplier<BATCH_SIZE,OUTPUT_DIMENSION>	arr_mult1;
	ones_array<BATCH_SIZE,OUTPUT_DIMENSION>			ones;
	array_subtractor<BATCH_SIZE,OUTPUT_DIMENSION>	subtract2;
	array_multiplier<BATCH_SIZE,OUTPUT_DIMENSION>	arr_mult2;

	SC_CTOR (calculate_final_delta) : subtract1("SUBTRACT1"), arr_mult1("ARR_MULT1"), ones("ONES"), subtract2("SUBTRACT2"), arr_mult2("ARR_MULT2") {
		for (int row = 0; row < BATCH_SIZE; row++) {
			for (int col = 0; col < OUTPUT_DIMENSION; col++) {
				// subtract1
				subtract1.input1[row][col](outf[row][col]);
				subtract1.input2[row][col](labels[row][col]);
				subtract1.output[row][col](subtract1_out[row][col]);
				// arr_mult1
				arr_mult1.input1[row][col](outf[row][col]);
				arr_mult1.input2[row][col](subtract1_out[row][col]);
				arr_mult1.output[row][col](arr_mult1_out[row][col]);
				// ones
				ones.output[row][col](ones_output[row][col]);
				// subtract2
				subtract2.input1[row][col](ones_output[row][col]);
				subtract2.input2[row][col](outf[row][col]);
				subtract2.output[row][col](subtract2_out[row][col]);
				// arr_mult2
				arr_mult2.input1[row][col](arr_mult1_out[row][col]);
				arr_mult2.input2[row][col](subtract2_out[row][col]);
				arr_mult2.output[row][col](output[row][col]);
			}
		}
	}
};

template <int HIDDEN_LAYER_DIMENSION, int BATCH_SIZE, OUTPUT_DIMENSION>
SC_MODULE (final_w_update) {

	// Ports
	sc_in<float>	out2[BATCH_SIZE][HIDDEN_LAYER_DIMENSION];
	sc_in<float>	final_delta[BATCH_SIZE][OUTPUT_DIMENSION];
	sc_out<float>	output[HIDDEN_LAYER_DIMENSION][OUTPUT_DIMENSION];

	// Signals
	sc_signal<float>	transpose_out[HIDDEN_LAYER_DIMENSION][BATCH_SIZE];

	// Sub-Modules
	matrix_transpose<BATCH_SIZE,HIDDEN_LAYER_DIMENSION>	transpose1;
	matrix_multiplier<HIDDEN_LAYER_DIMENSION, BATCH_SIZE, OUTPUT_DIMENSION>	mmult1;

	SC_CTOR (final_w_update) : transpose1("TRANSPOSE1"), mmult1("MMULT1") {
		for (int row = 0; row < BATCH_SIZE; row++) {
			for (int col = 0; col < HIDDEN_LAYER_DIMENSION; col++) {
				transpose1.input1[row][col](out2[row][col]);
			}
		}
		for (int row = 0; row < HIDDEN_LAYER_DIMENSION; row++) {
			for (int col = 0; col < BATCH_SIZE; col++) {
				transpose1.output[row][col](transpose_out[row][col]);
				mmult1.input1[row][col](transpose_out[row][col]);
			}
		}
		for (int row = 0; row < BATCH_SIZE; row++) {
			for (int col = 0; col < OUTPUT_DIMENSION; col++) {
				mmult1.input2[row][col](final_delta[row][col]);
			}
		}
		for (int row = 0; row < HIDDEN_LAYER_DIMENSION; row++) {
			for (int col = 0; col < OUTPUT_DIMENSION; col++) {
				mmult1.output[row][col](output[row][col]);
			}
		}
	}
}

#endif /* ANN.h */
