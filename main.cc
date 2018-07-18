#include <cstdlib>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <systemc.h>

#include "ANN.h"
#include "Constants.h"
#include "DataLoader.h"
#include "Modes.h"
#include "SystemCMatrix.h"

int sc_main(int argc, char* argv[]) {

	// Creating signals
	std::cout << "Initializing signals" << std::endl;
	sc_signal<float> 		input_data[BATCH_SIZE][INPUT_DIMENSION];
	sc_signal<float> 		input_labels[BATCH_SIZE][OUTPUT_DIMENSION];
	sc_signal<float>		input_weights1[INPUT_DIMENSION][HIDDEN_LAYER_DIMENSION];
	sc_signal<float>		input_weights2[HIDDEN_LAYER_DIMENSION][OUTPUT_DIMENSION];
	sc_signal<float>		output_weights1[INPUT_DIMENSION][HIDDEN_LAYER_DIMENSION];
	sc_signal<float>		output_weights2[HIDDEN_LAYER_DIMENSION][OUTPUT_DIMENSION];
	sc_signal<float>		fp_outf_debug[BATCH_SIZE][OUTPUT_DIMENSION];
	#if DEBUG == true
	sc_signal<float>		fp_out1_debug[BATCH_SIZE][INPUT_DIMENSION];
	sc_signal<float>		fp_out2_debug[BATCH_SIZE][HIDDEN_LAYER_DIMENSION];
	sc_signal<float>		fd_out_debug[BATCH_SIZE][OUTPUT_DIMENSION];
	sc_signal<float>		nfd_out_debug[BATCH_SIZE][HIDDEN_LAYER_DIMENSION];
	#endif 

	// Linking signals to back propogation module
	std::cout << "Initializing back-propogation model" << std::endl;
	back_propogation<BATCH_SIZE,INPUT_DIMENSION,HIDDEN_LAYER_DIMENSION,OUTPUT_DIMENSION> trainer("TRAINER");
	std::cout << "Linking signals" << std::endl;
	for (int row = 0; row < BATCH_SIZE; row++) {
		for (int col = 0; col < INPUT_DIMENSION; col++) {
			trainer.input_data[row][col](input_data[row][col]);
		}
	}
	for (int row = 0; row < BATCH_SIZE; row++) {
		for (int col = 0; col < OUTPUT_DIMENSION; col++) {
			trainer.input_labels[row][col](input_labels[row][col]);
			trainer.fp_outf_debug[row*OUTPUT_DIMENSION + col](fp_outf_debug[row][col]);
		}
	}
	for (int row = 0; row < INPUT_DIMENSION; row++) {
		for (int col = 0; col < HIDDEN_LAYER_DIMENSION; col++) {
			trainer.input_weights1[row][col](input_weights1[row][col]);
			trainer.output_weights1[row][col](output_weights1[row][col]);
		}
	}
	for (int row = 0; row < HIDDEN_LAYER_DIMENSION; row++) {
		for (int col = 0; col < OUTPUT_DIMENSION; col++) {
			trainer.input_weights2[row][col](input_weights2[row][col]);
			trainer.output_weights2[row][col](output_weights2[row][col]);
		}
	}
	#if DEBUG == true
	for (int row = 0; row < BATCH_SIZE; row++) {
		for (int col = 0; col < INPUT_DIMENSION; col++) {
			trainer.fp_out1_debug[row*INPUT_DIMENSION + col](fp_out1_debug[row][col]);
		}
	}
	for (int row = 0; row < BATCH_SIZE; row++) {
		for (int col = 0; col < HIDDEN_LAYER_DIMENSION; col++) {
			trainer.fp_out2_debug[row*HIDDEN_LAYER_DIMENSION + col](fp_out2_debug[row][col]);
			trainer.nfd_out_debug[row*HIDDEN_LAYER_DIMENSION + col](nfd_out_debug[row][col]);
		}
	}
	for (int row = 0; row < BATCH_SIZE; row++) {
		for (int col = 0; col < OUTPUT_DIMENSION; col++) {
			trainer.fd_out_debug[row*OUTPUT_DIMENSION + col](fd_out_debug[row][col]);
		}
	}
	#endif

	// Linking signals to trace file
	std::cout << "Creating trace file" << std::endl;
	sc_trace_file *trace_file = sc_create_vcd_trace_file("ARTIFICIAL NEURAL NETWORK");
	std::cout << "Linking signals to trace file" << std::endl;
	for (int row = 0; row < BATCH_SIZE; row++) {
		for (int col = 0; col < INPUT_DIMENSION; col++) {
			sc_trace(trace_file, input_data[row][col], "input_data");
		}
	}
	for (int row = 0; row < BATCH_SIZE; row++) {
		for (int col = 0; col < OUTPUT_DIMENSION; col++) {
			sc_trace(trace_file, input_labels[row][col], "input_labels");
			sc_trace(trace_file, fp_outf_debug[row][col], "fp_outf_debug");
		}
	}
	for (int row = 0; row < INPUT_DIMENSION; row++) {
		for (int col = 0; col < HIDDEN_LAYER_DIMENSION; col++) {
			sc_trace(trace_file, input_weights1[row][col], "input_weights1");
			sc_trace(trace_file, output_weights1[row][col], "output_weights1");
		}
	}
	for (int row = 0; row < HIDDEN_LAYER_DIMENSION; row++) {
		for (int col = 0; col < OUTPUT_DIMENSION; col++) {
			sc_trace(trace_file, input_weights2[row][col], "input_weights2");
			sc_trace(trace_file, output_weights2[row][col], "output_weights2");
		}
	}

	#if DEBUG == true
	for (int row = 0; row < BATCH_SIZE; row++) {
		for (int col = 0; col < INPUT_DIMENSION; col++) {
			sc_trace(trace_file, fp_out1_debug[row][col], "fp_out1_debug");
		}
	}
	for (int row = 0; row < BATCH_SIZE; row++) {
		for (int col = 0; col < HIDDEN_LAYER_DIMENSION; col++) {
			sc_trace(trace_file, fp_out2_debug[row][col], "fp_out2_debug");
			sc_trace(trace_file, nfd_out_debug[row][col], "nfd_out_debug");
		}
	}
	for (int row = 0; row < BATCH_SIZE; row++) {
		for (int col = 0; col < OUTPUT_DIMENSION; col++) {
			sc_trace(trace_file, fd_out_debug[row][col], "fd_out_debug");
		}
	}
	#endif

	// Loading batch data into the arrays and signals
	std::cout << "Loading data" << std::endl;
	float input_data_raw[BATCH_SIZE][INPUT_DIMENSION];
	float input_labels_raw[BATCH_SIZE][OUTPUT_DIMENSION];
	// Generating the data
	load_data(input_data_raw,input_labels_raw);
	// Sending data to signals
	for (int row = 0; row < BATCH_SIZE; row++) {
		for (int col = 0; col < INPUT_DIMENSION; col++) {
			input_data[row][col] = input_data_raw[row][col];
		}
	}
	for (int row = 0; row < BATCH_SIZE; row++) {
		for (int col = 0; col < OUTPUT_DIMENSION; col++) {
			input_labels[row][col] = input_labels_raw[row][col];
		}
	}
	
	// Generating randomized weights
	std::cout << "Generating weights" << std::endl;
	float weights1_raw[INPUT_DIMENSION][HIDDEN_LAYER_DIMENSION];
	float weights2_raw[HIDDEN_LAYER_DIMENSION][OUTPUT_DIMENSION];
	// Generating the weights
	for (int row = 0; row < INPUT_DIMENSION; row++) {
		for (int col = 0; col < HIDDEN_LAYER_DIMENSION; col++) {
			weights1_raw[row][col] = (((float) rand())/((float) RAND_MAX));
		}
	}
	for (int row = 0; row < HIDDEN_LAYER_DIMENSION; row++) {
		for (int col = 0; col < OUTPUT_DIMENSION; col++) {
			weights2_raw[row][col] = (((float) rand())/((float) RAND_MAX));
		}
	}
	// Sending weights to signals
	for (int row = 0; row < INPUT_DIMENSION; row++) {
		for (int col = 0; col < HIDDEN_LAYER_DIMENSION; col++) {
			input_weights1[row][col] = weights1_raw[row][col];
		}
	}
	for (int row = 0; row < HIDDEN_LAYER_DIMENSION; row++) {
		for (int col = 0; col < OUTPUT_DIMENSION; col++) {
			input_weights2[row][col] = weights2_raw[row][col];
		}
	}

	// Training loop
	std::cout << "Commencing training" << std::endl;
	int count = 0;
	int error = 0;
	while (count < 1000) {
		sc_start(1,SC_MS);

		// Weight adjustment
		for (int row = 0; row < INPUT_DIMENSION; row++) {
			for (int col = 0; col < HIDDEN_LAYER_DIMENSION; col++) {
				input_weights1[row][col] = input_weights1[row][col] - (0.1*output_weights1[row][col]);
			}
		}
		for (int row = 0; row < HIDDEN_LAYER_DIMENSION; row++) {
			for (int col = 0; col < OUTPUT_DIMENSION; col++) {
				input_weights2[row][col] = input_weights2[row][col] - (0.1*output_weights2[row][col]);
			}
		}

		// Calculating R^2 error
		error = 0;
		for (int row = 0; row < BATCH_SIZE; row++) {
			for (int col = 0; col < OUTPUT_DIMENSION; col++) {
				float element_error = ((float) input_labels[row][col]) - ((float) fp_outf_debug[row][col]);
				error += element_error * element_error;
			}
		}
		std::cout << fp_outf_debug[0][0] << std::endl;

		// Calculating % Correct

		count++;
	}

	sc_close_vcd_trace_file(trace_file);

	return 0;
}
