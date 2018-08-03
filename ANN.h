#ifndef _ANN_H_
#define _ANN_H_

#include <iostream>
#include <systemc.h>

#include "SystemCMatrix.h"

template <int BATCH_SIZE, int INPUT_DIMENSION, int HIDDEN_LAYER_DIMENSION, int OUTPUT_DIMENSION>
SC_MODULE (feed_forward) {

	// Ports
	sc_in<double>		input[BATCH_SIZE][INPUT_DIMENSION];
	sc_in<double>		weights1[INPUT_DIMENSION][HIDDEN_LAYER_DIMENSION];
	sc_in<double>		weights2[HIDDEN_LAYER_DIMENSION][OUTPUT_DIMENSION];

	sc_out<double>		out1[BATCH_SIZE][INPUT_DIMENSION];
	sc_out<double>		out2[BATCH_SIZE][HIDDEN_LAYER_DIMENSION];
	sc_out<double>		outf[BATCH_SIZE][OUTPUT_DIMENSION];

	// Signals
	sc_signal<double>	mmult_out1[BATCH_SIZE][HIDDEN_LAYER_DIMENSION];
	sc_signal<double>	mmult_out2[BATCH_SIZE][OUTPUT_DIMENSION];
	sc_signal<double>	sig_out1[BATCH_SIZE][HIDDEN_LAYER_DIMENSION];
	sc_signal<double>	sig_out2[BATCH_SIZE][OUTPUT_DIMENSION];

	// Sub-Modules
	matrix_multiplier<BATCH_SIZE,INPUT_DIMENSION,HIDDEN_LAYER_DIMENSION>	mmult1;
	sigmoid_activation<BATCH_SIZE,HIDDEN_LAYER_DIMENSION>					sig_act1;
	matrix_multiplier<BATCH_SIZE,HIDDEN_LAYER_DIMENSION,OUTPUT_DIMENSION>	mmult2;
	sigmoid_activation<BATCH_SIZE,OUTPUT_DIMENSION>							sig_act2;

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
					out2[row][col].write(sig_out1[row][col].read());
				}
		}
	}

	void set_outf() {
		for (int row = 0; row < BATCH_SIZE; row++) {
				for (int col = 0; col < OUTPUT_DIMENSION; col++) {
					outf[row][col].write(sig_out2[row][col].read());
				}
		}
	}

	// Signals
	SC_CTOR (feed_forward) : mmult1("MMULT1"), mmult2("MMULT2"), sig_act1("SIG_ACT1"), sig_act2("SIG_ACT2") {
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
					sensitive << sig_out1[row][col];
				}
			}
		SC_METHOD (set_outf);
			dont_initialize();
			for (int row = 0; row < BATCH_SIZE; row++) {
				for (int col = 0; col < OUTPUT_DIMENSION; col++) {
					sensitive << sig_out2[row][col];
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
				mmult1.output[row][col](mmult_out1[row][col]);
			}
		}
		// mmult2
		for (int row = 0; row < BATCH_SIZE; row++) {
			for (int col = 0; col < HIDDEN_LAYER_DIMENSION; col++) {
				mmult2.input1[row][col](sig_out1[row][col]);
			}
		}
		for (int row = 0; row < HIDDEN_LAYER_DIMENSION; row++) {
			for (int col = 0; col < OUTPUT_DIMENSION; col++) {
				mmult2.input2[row][col](weights2[row][col]);
			}
		}
		for (int row = 0; row < BATCH_SIZE; row++) {
			for (int col = 0; col < OUTPUT_DIMENSION; col++) {
				mmult2.output[row][col](mmult_out2[row][col]);
			}
		}
		// sig_act1
		for (int row = 0; row < BATCH_SIZE; row++) {
			for (int col = 0; col < HIDDEN_LAYER_DIMENSION; col++) {
				sig_act1.input[row][col](mmult_out1[row][col]);
				sig_act1.output[row][col](sig_out1[row][col]);
			}
		}
		// sig_act2
		for (int row = 0; row < BATCH_SIZE; row++) {
			for (int col = 0; col < OUTPUT_DIMENSION; col++) {
				sig_act2.input[row][col](mmult_out2[row][col]);
				sig_act2.output[row][col](sig_out2[row][col]);
			}
		}
	}
};

template <int BATCH_SIZE, int OUTPUT_DIMENSION>
// NOTE: This current implementation is based on the assumption that there will only be one hidden layer
SC_MODULE (calculate_final_delta) {

	// Ports
	sc_in<double>		labels[BATCH_SIZE][OUTPUT_DIMENSION];
	sc_in<double>		outf[BATCH_SIZE][OUTPUT_DIMENSION];
	sc_out<double>		output[BATCH_SIZE][OUTPUT_DIMENSION];

	// Signals
	sc_signal<double>	subtract1_out[BATCH_SIZE][OUTPUT_DIMENSION];
	sc_signal<double>	arr_mult1_out[BATCH_SIZE][OUTPUT_DIMENSION];
	sc_signal<double>	ones_output[BATCH_SIZE][OUTPUT_DIMENSION];
	sc_signal<double>	subtract2_out[BATCH_SIZE][OUTPUT_DIMENSION];

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
	sc_in<double>	out2[BATCH_SIZE][HIDDEN_LAYER_DIMENSION];
	sc_in<double>	final_delta[BATCH_SIZE][OUTPUT_DIMENSION];
	sc_out<double>	output[HIDDEN_LAYER_DIMENSION][OUTPUT_DIMENSION];

	// Signals
	sc_signal<double>	transpose_out[HIDDEN_LAYER_DIMENSION][BATCH_SIZE];

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
	sc_in<double> later_delta[BATCH_SIZE][OUTPUT_DIMENSION];
	sc_in<double> later_weights[HIDDEN_LAYER_DIMENSION][OUTPUT_DIMENSION];
	sc_in<double> previous_output[BATCH_SIZE][HIDDEN_LAYER_DIMENSION];
	sc_out<double> delta[BATCH_SIZE][HIDDEN_LAYER_DIMENSION];

	// Signals
	sc_signal<double> transpose1_out[OUTPUT_DIMENSION][HIDDEN_LAYER_DIMENSION];
	sc_signal<double> mmult1_out[BATCH_SIZE][HIDDEN_LAYER_DIMENSION];
	sc_signal<double> ones1_out[BATCH_SIZE][HIDDEN_LAYER_DIMENSION];
	sc_signal<double> arr_mult1_out[BATCH_SIZE][HIDDEN_LAYER_DIMENSION];
	sc_signal<double> subtract1_out[BATCH_SIZE][HIDDEN_LAYER_DIMENSION];

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
	sc_core::sc_in<double> 		input_data[BATCH_SIZE][INPUT_DIMENSION];
	sc_core::sc_in<double> 		input_labels[BATCH_SIZE][OUTPUT_DIMENSION];
	sc_core::sc_in<double>		input_weights1[INPUT_DIMENSION][HIDDEN_LAYER_DIMENSION];
	sc_core::sc_in<double>		input_weights2[HIDDEN_LAYER_DIMENSION][OUTPUT_DIMENSION];
	sc_core::sc_out<double>		output_weights1[INPUT_DIMENSION][HIDDEN_LAYER_DIMENSION];
	sc_core::sc_out<double>		output_weights2[HIDDEN_LAYER_DIMENSION][OUTPUT_DIMENSION];
	sc_core::sc_vector<sc_core::sc_out<double> >	fp_outf_debug;

	void set_fp_outf_debug() {
		for (int row = 0; row < BATCH_SIZE; row++) {
			for (int col = 0; col < OUTPUT_DIMENSION; col++) {
				fp_outf_debug[row*OUTPUT_DIMENSION + col].write(fp_outf[row][col].read());
			}
		}
	}

	// Signals
	sc_signal<double>	fp_out1[BATCH_SIZE][INPUT_DIMENSION];
	sc_signal<double>	fp_out2[BATCH_SIZE][HIDDEN_LAYER_DIMENSION];
	sc_signal<double>	fp_outf[BATCH_SIZE][OUTPUT_DIMENSION];
	sc_signal<double>	fd_out[BATCH_SIZE][OUTPUT_DIMENSION];
	sc_signal<double>	nfd_out[BATCH_SIZE][HIDDEN_LAYER_DIMENSION];

	// Sub-Modules
	feed_forward<BATCH_SIZE,INPUT_DIMENSION,HIDDEN_LAYER_DIMENSION,OUTPUT_DIMENSION>	forward_propogate;
	calculate_final_delta<BATCH_SIZE,OUTPUT_DIMENSION>					final_delta;
	calculate_non_final_delta<BATCH_SIZE,OUTPUT_DIMENSION,HIDDEN_LAYER_DIMENSION>		non_final_delta;
	weight_update<INPUT_DIMENSION,BATCH_SIZE,HIDDEN_LAYER_DIMENSION>			weight1_update;
	weight_update<HIDDEN_LAYER_DIMENSION,BATCH_SIZE,OUTPUT_DIMENSION>			weight2_update;

	SC_CTOR (back_propogation) : 
		forward_propogate("FORWARD_PROPOGATE"), 
		final_delta("FINAL_DELTA"), 
		non_final_delta("NON_FINAL_DELTA"),
		weight1_update("WEIGHT1_UPDATE"),
		fp_outf_debug("FP_OUTF_DEBUG", BATCH_SIZE * OUTPUT_DIMENSION),
		weight2_update("WEIGHT2_UPDATE")
	{

		SC_METHOD(set_fp_outf_debug);
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
				// forward_propogate.out1[row][col](fp_out1_debug[row*INPUT_DIMENSION + col]);
				// weight1_update
				weight1_update.out2[row][col](fp_out1[row][col]);
			}
		}
		for (int row = 0; row < BATCH_SIZE; row++) {
			for (int col = 0; col < HIDDEN_LAYER_DIMENSION; col++) {
				// forward_propogate
				forward_propogate.out2[row][col](fp_out2[row][col]);
				// forward_propogate.out2[row][col](fp_out2_debug[row*HIDDEN_LAYER_DIMENSION + col]);
				// non_final_delta
				non_final_delta.previous_output[row][col](fp_out2[row][col]);
				non_final_delta.delta[row][col](nfd_out[row][col]);
				// non_final_delta.delta[row][col](nfd_out_debug[row*HIDDEN_LAYER_DIMENSION + col]);
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

template <	int BATCH_SIZE, \
			int TEST_BATCH_SIZE, \
			int INPUT_DIMENSION, \
			int HIDDEN_LAYER_DIMENSION, \
			int OUTPUT_DIMENSION \
		>
SC_MODULE (ann_trainer) {

	// Data Ports
	sc_in	<bool>			clk;
	sc_in	<double>		input_data[BATCH_SIZE][INPUT_DIMENSION];
	sc_in	<double>		input_labels[BATCH_SIZE][OUTPUT_DIMENSION];
	sc_in	<double>		input_test_data[TEST_BATCH_SIZE][INPUT_DIMENSION];
	sc_in	<double>		input_test_labels[TEST_BATCH_SIZE][OUTPUT_DIMENSION];

	// Specification Ports
	sc_in	<int>			stop_iteration;
	sc_in	<double>		stop_percent;
	sc_in	<double>		stop_cost;
	sc_in	<double>		stop_time;

	// BP Signals
	sc_signal	<double>	input_weights1[INPUT_DIMENSION][HIDDEN_LAYER_DIMENSION];
	sc_signal	<double>	input_weights2[HIDDEN_LAYER_DIMENSION][OUTPUT_DIMENSION];
	sc_signal	<double>	output_weights1[INPUT_DIMENSION][HIDDEN_LAYER_DIMENSION];
	sc_signal	<double>	output_weights2[HIDDEN_LAYER_DIMENSION][OUTPUT_DIMENSION];
	sc_signal 	<double>	fp_outf_debug[BATCH_SIZE][OUTPUT_DIMENSION];

	// FF Signals
	sc_signal	<double>	ff_out1[TEST_BATCH_SIZE][INPUT_DIMENSION];
	sc_signal	<double>	ff_out2[TEST_BATCH_SIZE][HIDDEN_LAYER_DIMENSION];
	sc_signal	<double>	ff_outf[TEST_BATCH_SIZE][OUTPUT_DIMENSION];

	// Local Variables
	int iteration;
	double error;
	double previous_error;
	double correct;
	clock_t starting_time;
	clock_t previous_time;
	clock_t current_time;

	// Sub Modules
	back_propogation<BATCH_SIZE,INPUT_DIMENSION,HIDDEN_LAYER_DIMENSION,OUTPUT_DIMENSION> back_propogation_module;
	feed_forward<TEST_BATCH_SIZE, INPUT_DIMENSION, HIDDEN_LAYER_DIMENSION, OUTPUT_DIMENSION> feed_forward_module;

	// Methods
	void initialize () {
		std::cout << "Initializing BP module" << std::endl;

		// Generating weights
		for (int row = 0; row < INPUT_DIMENSION; row++) {
			for (int col = 0; col < HIDDEN_LAYER_DIMENSION; col++) {
				input_weights1[row][col].write( ((double) rand()) / ((double) RAND_MAX / 2) - 1 );
			}
		}
		for (int row = 0; row < HIDDEN_LAYER_DIMENSION; row++) {
			for (int col = 0; col < OUTPUT_DIMENSION; col++) {
				input_weights2[row][col].write( ((double) rand()) / ((double) RAND_MAX / 2) - 1 );
			}
		}
		std::cout << "Random weights generated" << std::endl;

		// Loading local variables
		iteration = 0;
		error = 10000;
		previous_error = 0;
		correct = 0;
		starting_time = clock();
		previous_time = clock();
		current_time = clock();
		std::cout << "Local variables loaded" << std::endl;
	}

	void step () {
		// Weight adjustment
		for (int row = 0; row < INPUT_DIMENSION; row++) {
			for (int col = 0; col < HIDDEN_LAYER_DIMENSION; col++) {
				input_weights1[row][col].write(input_weights1[row][col].read() - (0.03*output_weights1[row][col].read()));
			}
		}
		for (int row = 0; row < HIDDEN_LAYER_DIMENSION; row++) {
			for (int col = 0; col < OUTPUT_DIMENSION; col++) {
				input_weights2[row][col].write(input_weights2[row][col].read() - (0.03*output_weights2[row][col].read()));
			}
		}

		// Calculating sum error squared
		previous_error = error;
		error = 0;
		for (int row = 0; row < BATCH_SIZE; row++) {
			for (int col = 0; col < OUTPUT_DIMENSION; col++) {
				double element_error = ((double) input_labels[row][col].read()) - ((double) fp_outf_debug[row][col].read());
				error += element_error * element_error;
			}
		}
		error /= BATCH_SIZE * OUTPUT_DIMENSION;

		// Calculating % Correct
		correct = 0;
		for (int row = 0; row < BATCH_SIZE; row++) {
			// Per row data
			double max_value = 0;
			int max_value_index = 0;
			for (int col = 0; col < OUTPUT_DIMENSION; col++) {
				double value = fp_outf_debug[row][col].read();
				if (value > max_value) {
					max_value_index = col;
					max_value = value;
				}
			}
			if (input_labels[row][max_value_index].read() == 1) {
				correct++;
			}
		}
		
		iteration++;

		previous_time = current_time;
		current_time = clock();

		// Iteration information
		std::cout << "Iteration: " << std::setw(6) << std::left << iteration << " ";
		std::cout << "Correct: " << std::setw(3) << std::right << correct << "/" << BATCH_SIZE <<  " ";
		std::cout << "Sum-Error^2: " << std::setw(10) << std::left << error << " ";
		std::cout << "Delta-Error: " << std::setw(12) << std::left << error - previous_error << " ";
		std::cout << "Time: " << std::setw(8) << std::left << ((double) current_time - (double) starting_time) / CLOCKS_PER_SEC / 60 << " ";
		std::cout << "Delta-Time: " << std::setw(8) << std::left << ((double) current_time - (double) previous_time) / CLOCKS_PER_SEC << " ";
		std::cout << std::endl;

		// Termination
		bool case1 = iteration < stop_iteration.read();
		bool case2 = correct / BATCH_SIZE < stop_percent.read();
		bool case3 = error > stop_cost.read();
		bool case4 = (current_time - starting_time) / CLOCKS_PER_SEC / 60 < stop_time.read();
		if (case1 && case2 && case3 && case4) {
			// Continue
		} else {
			if (!case1) {
				std::cout << "Training terminated: Max iteration reached." << std::endl;
			}
			if (!case2) {
				std::cout << "Training terminated: Halting percentage reached." << std::endl;
			}
			if (!case3) {
				std::cout << "Training termianted: Minimum cost reached." << std::endl;
			}
			if (!case4) {
				std::cout << "Training terminated: Max time reached." << std::endl;
			}
			sc_stop();
		}
	}

	void test() {
		std::cout << "Commencing testing" << std::endl;

		double test_error = 0;
		for (int row = 0; row < TEST_BATCH_SIZE; row++) {
			for (int col = 0; col < OUTPUT_DIMENSION; col++) {
				double element_error = ((double) input_test_labels[row][col].read()) - ((double) ff_outf[row][col].read());
				test_error += element_error * element_error;
			}
		}
		test_error /= TEST_BATCH_SIZE * OUTPUT_DIMENSION;
		std::cout << "TEST ERROR: " << test_error << std::endl;

		// Calculating % Correct
		correct = 0;
		for (int row = 0; row < TEST_BATCH_SIZE; row++) {
			// Per row data
			double max_value = 0;
			int max_value_index = 0;
			for (int col = 0; col < OUTPUT_DIMENSION; col++) {
				double value = ff_outf[row][col].read();
				if (value > max_value) {
					max_value_index = col;
					max_value = value;
				}
			}
			if (input_test_labels[row][max_value_index].read() == 1) {
				correct++;
			}
		}
		std::cout << "TEST_CORRECT: " << correct << "/10" << std::endl;

	}

	SC_CTOR (ann_trainer) : back_propogation_module("BACK_PROPOGATION_MODULE"), feed_forward_module("FEED_FORWARD_MODULE") {
		// Methods
		SC_METHOD (initialize);
			dont_initialize();
		SC_METHOD (step);
			sensitive << clk;
		SC_METHOD (test);
			dont_initialize();
		// Binding BP Ports
		for (int row = 0; row < BATCH_SIZE; row++) {
			for (int col = 0; col < INPUT_DIMENSION; col++) {
				back_propogation_module.input_data[row][col](input_data[row][col]);
			}
		}
		for (int row = 0; row < BATCH_SIZE; row++) {
			for (int col = 0; col < OUTPUT_DIMENSION; col++) {
				back_propogation_module.input_labels[row][col](input_labels[row][col]);
				back_propogation_module.fp_outf_debug[row*OUTPUT_DIMENSION + col](fp_outf_debug[row][col]);
			}
		}
		for (int row = 0; row < INPUT_DIMENSION; row++) {
			for (int col = 0; col < HIDDEN_LAYER_DIMENSION; col++) {
				back_propogation_module.input_weights1[row][col](input_weights1[row][col]);
				back_propogation_module.output_weights1[row][col](output_weights1[row][col]);
			}
		}
		for (int row = 0; row < HIDDEN_LAYER_DIMENSION; row++) {
			for (int col = 0; col < OUTPUT_DIMENSION; col++) {
				back_propogation_module.input_weights2[row][col](input_weights2[row][col]);
				back_propogation_module.output_weights2[row][col](output_weights2[row][col]);
			}
		}
		// Binding FF Ports
		for (int row = 0; row < TEST_BATCH_SIZE; row++) {
			for (int col = 0; col < INPUT_DIMENSION; col++) {
				feed_forward_module.out1[row][col](ff_out1[row][col]);
				feed_forward_module.input[row][col](input_test_data[row][col]);
			}
		}
		for (int row = 0; row < TEST_BATCH_SIZE; row++) {
			for (int col = 0; col < HIDDEN_LAYER_DIMENSION; col++) {
				feed_forward_module.out2[row][col](ff_out2[row][col]);
			}
		}
		for (int row = 0; row < TEST_BATCH_SIZE; row++) {
			for (int col = 0; col < OUTPUT_DIMENSION; col++) {
				feed_forward_module.outf[row][col](ff_outf[row][col]);
			}
		}
		for (int row = 0; row < INPUT_DIMENSION; row++) {
			for (int col = 0; col < HIDDEN_LAYER_DIMENSION; col++) {
				feed_forward_module.weights1[row][col](input_weights1[row][col]);
			}
		}
		for (int row = 0; row < HIDDEN_LAYER_DIMENSION; row++) {
			for (int col = 0; col < OUTPUT_DIMENSION; col++) {
				feed_forward_module.weights2[row][col](input_weights2[row][col]);
			}
		}
	}
};

#endif /* ANN.h */
