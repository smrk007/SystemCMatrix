#include <cstdlib>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <systemc.h>

#include "SystemCMatrix.h"
#include "ANN.h"
#include "DataLoader.h"
#include "Constants.h"

int sc_main(int argc, char* argv[]) {

	// Creating signals
	sc_signal<float> 		input_data[BATCH_SIZE][INPUT_DIMENSION];
	sc_signal<float> 		input_labels[BATCH_SIZE][OUTPUT_DIMENSION];
	sc_signal<float>		input_weights1[INPUT_DIMENSION][HIDDEN_LAYER_DIMENSION];
	sc_signal<float>		input_weights2[HIDDEN_LAYER_DIMENSION][OUTPUT_DIMENSION];
	sc_signal<float>		output_weights1[INPUT_DIMENSION][HIDDEN_LAYER_DIMENSION];
	sc_signal<float>		output_weights2[HIDDEN_LAYER_DIMENSION][OUTPUT_DIMENSION];

	// Linking signals to back propogation module
	back_propogation<BATCH_SIZE,INPUT_DIMENSION,HIDDEN_LAYER_DIMENSION,OUTPUT_DIMENSION> trainer("TRAINER");
	for (int row = 0; row < BATCH_SIZE; row++) {
		for (int col = 0; col < INPUT_DIMENSION; col++) {
			trainer.input_data[row][col](input_data[row][col]);
		}
	}
	for (int row = 0; row < BATCH_SIZE; row++) {
		for (int col = 0; col < OUTPUT_DIMENSION; col++) {
			trainer.input_labels[row][col](input_labels[row][col]);
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

	// Linking signals to trace file
	sc_trace_file *trace_file = sc_create_vcd_trace_file("ARTIFICIAL NEURAL NETWORK");
	for (int row = 0; row < BATCH_SIZE; row++) {
		for (int col = 0; col < INPUT_DIMENSION; col++) {
			sc_trace(trace_file, input_data[row][col], "input_data");
		}
	}
	for (int row = 0; row < BATCH_SIZE; row++) {
		for (int col = 0; col < OUTPUT_DIMENSION; col++) {
			sc_trace(trace_file, input_labels[row][col], "input_labels");
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
	

	// Loading batch data into the arrays and signals
	float input_data_raw[BATCH_SIZE][INPUT_DIMENSION];
	float input_labels_raw[BATCH_SIZE][OUTPUT_DIMENSION];
	load_data(input_data_raw,input_labels_raw);
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
	float weights1_raw[INPUT_DIMENSION][HIDDEN_LAYER_DIMENSION];
	float weights2_raw[HIDDEN_LAYER_DIMENSION][OUTPUT_DIMENSION];
	for (int row = 0; row < INPUT_DIMENSION; row++) {
		for (int col = 0; col < HIDDEN_LAYER_DIMENSION; col++) {
			weights1_raw[row][col] = (((float) rand())/((float) RAND_MAX)) - 0.5;
		}
	}
	for (int row = 0; row < HIDDEN_LAYER_DIMENSION; row++) {
		for (int col = 0; col < OUTPUT_DIMENSION; col++) {
			weights2_raw[row][col] = (((float) rand())/((float) RAND_MAX)) - 0.5;
		}
	}
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

	return 0;
}
