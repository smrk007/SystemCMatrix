#ifndef _ANN_H_
#define _ANN_H_

#include <iostream>
#include <systemc.h>
#include "SystemCMatrix.h"

template <int BATCH_SIZE, int INPUT_DIMENSION, int HIDDEN_LAYER_DIMENSION, int OUTPUT_DIMENSION>
SC_MODULE (feed_forward) {

	// Ports
	sc_in<float>		input[BATCH_SIZE][INPUT_DIMENSION];
	sc_in<float>		weights1[INPUT_DIMENSION][HIDDEN_LAYER_DIMENSION];
	sc_in<float>		weights2[HIDDEN_LAYER_DIMENSION][OUTPUT_DIMENSION];

	sc_out<float>		out1[BATCH_SIZE][INPUT_DIMENSION];
	sc_out<float>		out2[BATCH_SIZE][HIDDEN_LAYER_DIMENSION];
	sc_out<float>		outf[BATCH_SIZE][OUTPUT_DIMENSION];

	// Signals
	sc_signal<float>	mult_out1[BATCH_SIZE][HIDDEN_LAYER_DIMENSION];
	sc_signal<float>	mult_out2[BATCH_SIZE][HIDDEN_LAYER_DIMENSION];

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
	}

	void set_out2() {
		for (int row = 0; row < BATCH_SIZE; row++) {
				for (int col = 0; col < HIDDEN_LAYER_DIMENSION; col++) {
					out2[row][col].write(mult_out1[row][col].read());
				}
		}
	}

	void set_outf() {
		for (int row = 0; row < BATCH_SIZE; row++) {
				for (int col = 0; col < OUTPUT_DIMENSION; col++) {
					outf[row][col].write(mult_out2[row][col].read());
				}
		}
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

template <int HIDDEN_LAYER_DIMENSION, int BATCH_SIZE, int OUTPUT_DIMENSION>
SC_MODULE (weight_update) {

	// Ports
	sc_in<float>	out2[BATCH_SIZE][HIDDEN_LAYER_DIMENSION];
	sc_in<float>	final_delta[BATCH_SIZE][OUTPUT_DIMENSION];
	sc_out<float>	output[HIDDEN_LAYER_DIMENSION][OUTPUT_DIMENSION];

	// Signals
	sc_signal<float>	transpose_out[HIDDEN_LAYER_DIMENSION][BATCH_SIZE];

	// Sub-Modules
	matrix_transpose<BATCH_SIZE,HIDDEN_LAYER_DIMENSION>	transpose1;
	matrix_multiplier<HIDDEN_LAYER_DIMENSION, BATCH_SIZE, OUTPUT_DIMENSION>	mmult1;

	SC_CTOR (weight_update) : transpose1("TRANSPOSE1"), mmult1("MMULT1") {
		for (int row = 0; row < BATCH_SIZE; row++) {
			for (int col = 0; col < HIDDEN_LAYER_DIMENSION; col++) {
				transpose1.input[row][col](out2[row][col]);
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
};

template <int BATCH_SIZE, int OUTPUT_DIMENSION, int HIDDEN_LAYER_DIMENSION>
SC_MODULE (calculate_non_final_delta) {

	// Ports
	sc_in<float> later_delta[BATCH_SIZE][OUTPUT_DIMENSION];
	sc_in<float> later_weights[HIDDEN_LAYER_DIMENSION][OUTPUT_DIMENSION];
	sc_in<float> previous_output[BATCH_SIZE][HIDDEN_LAYER_DIMENSION];
	sc_out<float> delta[BATCH_SIZE][HIDDEN_LAYER_DIMENSION];

	// Signals
	sc_signal<float> transpose1_out[OUTPUT_DIMENSION][HIDDEN_LAYER_DIMENSION];
	sc_signal<float> mmult1_out[BATCH_SIZE][HIDDEN_LAYER_DIMENSION];
	sc_signal<float> ones1_out[BATCH_SIZE][HIDDEN_LAYER_DIMENSION];
	sc_signal<float> arr_mult1_out[BATCH_SIZE][HIDDEN_LAYER_DIMENSION];
	sc_signal<float> subtract1_out[BATCH_SIZE][HIDDEN_LAYER_DIMENSION];

	// Sub Modules
	matrix_transpose<HIDDEN_LAYER_DIMENSION,OUTPUT_DIMENSION>		transpose1;
	matrix_multiplier<BATCH_SIZE,OUTPUT_DIMENSION,HIDDEN_LAYER_DIMENSION>	mmult1;
	array_multiplier<BATCH_SIZE,HIDDEN_LAYER_DIMENSION>		arr_mult1;
	ones_array<BATCH_SIZE,HIDDEN_LAYER_DIMENSION>			ones1;
	array_subtractor<BATCH_SIZE,HIDDEN_LAYER_DIMENSION>		subtract1;
	array_multiplier<BATCH_SIZE,HIDDEN_LAYER_DIMENSION> 	arr_mult2;

	SC_CTOR (calculate_non_final_delta) : transpose1("TRANSPOSE1"), mmult1("MMULT1"), arr_mult1("ARR_MULT1"), ones1("ARR_ONES1"), subtract1("SUBTRACT1"), arr_mult2("ARR_MULT2") {
		for (int row = 0; row < HIDDEN_LAYER_DIMENSION; row++) {
			for (int col = 0; col < OUTPUT_DIMENSION; col++) {
				// transpose1
				transpose1.input[row][col](later_weights[row][col]);
			}
		}
		for (int row = 0; row < OUTPUT_DIMENSION; row++) {
			for (int col = 0; col < HIDDEN_LAYER_DIMENSION; col++) {
				// transpose1
				transpose1.output[row][col](transpose1_out[row][col]);
				// mmult1
				mmult1.input2[row][col](transpose1_out[row][col]);
			}
		}
		for (int row = 0; row < BATCH_SIZE; row++) {
			for (int col = 0; col < OUTPUT_DIMENSION; col++) {
				// mmult1
				mmult1.input1[row][col](later_delta[row][col]);
			}
		}
		for (int row = 0; row < BATCH_SIZE; row++) {
			for (int col = 0; col < HIDDEN_LAYER_DIMENSION; col++) {
				// mmult1
				mmult1.output[row][col](mmult1_out[row][col]);
				// arr_mult1
				arr_mult1.input1[row][col](mmult1_out[row][col]);
				arr_mult1.input2[row][col](previous_output[row][col]);
				arr_mult1.output[row][col](arr_mult1_out[row][col]);
				// ones1
				ones1.output[row][col](ones1_out[row][col]);
				// subtract1
				subtract1.input1[row][col](ones1_out[row][col]);
				subtract1.input2[row][col](previous_output[row][col]);
				subtract1.output[row][col](subtract1_out[row][col]);
				// arr_mult2
				arr_mult2.input1[row][col](arr_mult1_out[row][col]);
				arr_mult2.input2[row][col](subtract1_out[row][col]);
				arr_mult2.output[row][col](delta[row][col]);
			}
		}
	}
};

template <int BATCH_SIZE, int INPUT_DIMENSION, int HIDDEN_LAYER_DIMENSION, int OUTPUT_DIMENSION>
SC_MODULE (back_propogation) {

	// Ports
	sc_in<float> 		input_data[BATCH_SIZE][INPUT_DIMENSION];
	sc_in<float> 		input_labels[BATCH_SIZE][OUTPUT_DIMENSION];
	sc_in<float>		input_weights1[INPUT_DIMENSION][HIDDEN_LAYER_DIMENSION];
	sc_in<float>		input_weights2[HIDDEN_LAYER_DIMENSION][OUTPUT_DIMENSION];
	sc_out<float>		output_weights1[INPUT_DIMENSION][HIDDEN_LAYER_DIMENSION];
	sc_out<float>		output_weights2[HIDDEN_LAYER_DIMENSION][OUTPUT_DIMENSION];
	sc_out<float>		output_labels[BATCH_SIZE][OUTPUT_DIMENSION];

	// Signals
	sc_signal<float>	fp_out1[BATCH_SIZE][INPUT_DIMENSION];
	sc_signal<float>	fp_out2[BATCH_SIZE][HIDDEN_LAYER_DIMENSION];
	sc_signal<float>	fp_outf[BATCH_SIZE][OUTPUT_DIMENSION];
	sc_signal<float>	fd_out[BATCH_SIZE][OUTPUT_DIMENSION];
	sc_signal<float>	nfd_out[BATCH_SIZE][HIDDEN_LAYER_DIMENSION];

	// Sub-Modules
	feed_forward<BATCH_SIZE,INPUT_DIMENSION,HIDDEN_LAYER_DIMENSION,OUTPUT_DIMENSION>	forward_propogate;
	calculate_final_delta<BATCH_SIZE,OUTPUT_DIMENSION>					final_delta;
	calculate_non_final_delta<BATCH_SIZE,OUTPUT_DIMENSION,HIDDEN_LAYER_DIMENSION>		non_final_delta;
	weight_update<INPUT_DIMENSION,BATCH_SIZE,HIDDEN_LAYER_DIMENSION>			weight1_update;
	weight_update<HIDDEN_LAYER_DIMENSION,BATCH_SIZE,OUTPUT_DIMENSION>			weight2_update;

	// Methods
	void feedforward_output() {
		for (int row = 0; row < BATCH_SIZE; row++) {
			for (int col = 0; col < OUTPUT_DIMENSION; col++) {
				output_labels[row][col].write(fp_outf[row][col].read());
			}
		}
	}

	SC_CTOR (back_propogation) : forward_propogate("FORWARD_PROPOGATE"), final_delta("FINAL_DELTA"), non_final_delta("NON_FINAL_DELTA"), weight1_update("WEIGHT1_UPDATE"), weight2_update("WEIGHT2_UPDATE") {
		SC_METHOD (feedforward_output);
		dont_initialize();
		for (int row = 0; row < BATCH_SIZE; row++) {
			for (int col = 0; col < OUTPUT_DIMENSION; col++) {
				sensitive << fp_outf[row][col];
			}
		}
		// Linking components together
		for (int row = 0; row < BATCH_SIZE; row++) {
			for (int col = 0; col < INPUT_DIMENSION; col++) {
				// forward_propogate
				forward_propogate.input[row][col](input_data[row][col]);
				forward_propogate.out1[row][col](fp_out1[row][col]);
				// weight1_update
				weight1_update.out2[row][col](fp_out1[row][col]);
			}
		}
		for (int row = 0; row < BATCH_SIZE; row++) {
			for (int col = 0; col < HIDDEN_LAYER_DIMENSION; col++) {
				// forward_propogate
				forward_propogate.out2[row][col](fp_out2[row][col]);
				// non_final_delta
				non_final_delta.previous_output[row][col](fp_out2[row][col]);
				non_final_delta.delta[row][col](nfd_out[row][col]);
				// weight1_update
				weight1_update.final_delta[row][col](nfd_out[row][col]);
				// weight2_update
				weight2_update.out2[row][col](fp_out2[row][col]);
			}
		}
		for (int row = 0; row < BATCH_SIZE; row++) {
			for (int col = 0; col < OUTPUT_DIMENSION; col++) {
				// forward_propogate
				forward_propogate.outf[row][col](fp_outf[row][col]);
				// final_delta
				final_delta.labels[row][col](input_labels[row][col]);
				final_delta.outf[row][col](fp_outf[row][col]);
				final_delta.output[row][col](fd_out[row][col]);
				// non_final_delta
				non_final_delta.later_delta[row][col](fd_out[row][col]);
				// weight2_update
				weight2_update.final_delta[row][col](fd_out[row][col]);
			}
		}
		for (int row = 0; row < INPUT_DIMENSION; row++) {
			for (int col = 0; col < HIDDEN_LAYER_DIMENSION; col++) {
				// forward_propogate
				forward_propogate.weights1[row][col](input_weights1[row][col]);
				// weight1_update
				weight1_update.output[row][col](output_weights1[row][col]);
			}
		}
		for (int row = 0; row < HIDDEN_LAYER_DIMENSION; row++) {
			for (int col = 0; col < OUTPUT_DIMENSION; col++) {
				// forward_propogate
				forward_propogate.weights2[row][col](input_weights2[row][col]);
				// non_final_delta
				non_final_delta.later_weights[row][col](input_weights2[row][col]);
				// weight2_update
				weight2_update.output[row][col](output_weights2[row][col]);
			}
		}
	}
};

#endif /* ANN.h */
