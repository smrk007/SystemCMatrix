#include <cstdlib>
#include <ctime>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <systemc.h>

#include "ANN.h"
#include "Specs.h"
#include "DataLoader.h"
#include "SystemCMatrix.h"

int sc_main(int argc, char* argv[]) {

	// Creating signals
	std::cerr << "Initializing signals" << std::endl;
	sc_signal<double> 		input_data[BATCH_SIZE][INPUT_DIMENSION];
	sc_signal<double> 		input_labels[BATCH_SIZE][OUTPUT_DIMENSION];
	sc_signal<double>		input_weights1[INPUT_DIMENSION][HIDDEN_LAYER_DIMENSION];
	sc_signal<double>		input_weights2[HIDDEN_LAYER_DIMENSION][OUTPUT_DIMENSION];
	sc_signal<double>		output_weights1[INPUT_DIMENSION][HIDDEN_LAYER_DIMENSION];
	sc_signal<double>		output_weights2[HIDDEN_LAYER_DIMENSION][OUTPUT_DIMENSION];
	sc_signal<double>		fp_outf_debug[BATCH_SIZE][OUTPUT_DIMENSION];

	// Linking signals to back propogation module
	std::cerr << "Initializing back-propogation model" << std::endl;
	back_propogation<BATCH_SIZE,INPUT_DIMENSION,HIDDEN_LAYER_DIMENSION,OUTPUT_DIMENSION> trainer("TRAINER");
	std::cerr << "Linking signals" << std::endl;
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
	
	// Loading batch data into the arrays and signals
	std::cerr << "Loading data" << std::endl;
	double input_data_raw[BATCH_SIZE][INPUT_DIMENSION];
	double input_labels_raw[BATCH_SIZE][OUTPUT_DIMENSION];
	// Generating the data
	load_data(input_data_raw,input_labels_raw,DATA_FILE_PATH);
	// Sending data to signals
	for (int row = 0; row < BATCH_SIZE; row++) {
		for (int col = 0; col < INPUT_DIMENSION; col++) {
			input_data[row][col].write(input_data_raw[row][col]);
		}
	}
	for (int row = 0; row < BATCH_SIZE; row++) {
		for (int col = 0; col < OUTPUT_DIMENSION; col++) {
			input_labels[row][col].write(input_labels_raw[row][col]);
		}
	
	}
	// Generating randomized weights
	std::cerr << "Generating weights" << std::endl;
	double weights1_raw[INPUT_DIMENSION][HIDDEN_LAYER_DIMENSION];
	double weights2_raw[HIDDEN_LAYER_DIMENSION][OUTPUT_DIMENSION];
	// Generating the weights
	for (int row = 0; row < INPUT_DIMENSION; row++) {
		for (int col = 0; col < HIDDEN_LAYER_DIMENSION; col++) {
			weights1_raw[row][col] = ((double) rand()) / ((double) RAND_MAX / 2) - 1;
			input_weights1[row][col].write(weights1_raw[row][col]);
		}
	}
	for (int row = 0; row < HIDDEN_LAYER_DIMENSION; row++) {
		for (int col = 0; col < OUTPUT_DIMENSION; col++) {
			weights2_raw[row][col] = ((double) rand()) / ((double) RAND_MAX / 2) - 1;
			input_weights2[row][col].write(weights2_raw[row][col]);
		}
	}

	// Training loop
	std::cerr << "Commencing training" << std::endl;
	int iteration = 0;
	double error = 10000;
	double previous_error = 0;
	double correct = 0.0;
	clock_t starting_time = clock();
	clock_t previous_time = clock();
	clock_t current_time = clock();

	while 	(	\
			iteration < STOP_ITERATION && \
			correct/BATCH_SIZE < STOP_PERCENT && \
		error > STOP_COST && \
			(current_time - starting_time) / CLOCKS_PER_SEC / 60 < STOP_TIME \
	)
			
	{
		sc_start(1,SC_MS);
		// Weight adjustment
		for (int row = 0; row < INPUT_DIMENSION; row++) {
			for (int col = 0; col < HIDDEN_LAYER_DIMENSION; col++) {
				input_weights1[row][col].write(input_weights1[row][col].read() - (STEP_SIZE*output_weights1[row][col].read()));
			}
		}
		for (int row = 0; row < HIDDEN_LAYER_DIMENSION; row++) {
			for (int col = 0; col < OUTPUT_DIMENSION; col++) {
				input_weights2[row][col].write(input_weights2[row][col].read() - (STEP_SIZE*output_weights2[row][col].read()));
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
		std::cout << "Sum-Error^2: " << std::setw(8) << std::left << error << " ";
		std::cout << "Delta-Error: " << std::setw(9) << std::left << error - previous_error << " ";
		std::cout << "Time: " << std::setw(8) << std::left << ((double) current_time - (double) starting_time) / CLOCKS_PER_SEC / 60 << " ";
		std::cout << "Delta-Time: " << std::setw(8) << std::left << ((double) current_time - (double) previous_time) / CLOCKS_PER_SEC << " ";
		std::cout << std::endl;
	}
	std::cerr << "Training terminated" << std::endl;
	return 0;
}
