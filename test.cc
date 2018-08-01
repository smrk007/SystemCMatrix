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

int sc_main (int argc, char* argv[]) {

    // Signals to ports of ann_trainer
    sc_clock clk ("CLOCK", 1, 0, 0.5);
	sc_signal<double> 		input_data[BATCH_SIZE][INPUT_DIMENSION];
	sc_signal<double> 		input_labels[BATCH_SIZE][OUTPUT_DIMENSION];

    // Initialization of the module
    std::cerr << "Initializing the test" << std::endl;
    ann_trainer <BATCH_SIZE, INPUT_DIMENSION, HIDDEN_LAYER_DIMENSION, OUTPUT_DIMENSION> test_ann_trainer("TEST_ANN_TRAINER");
    test_ann_trainer.clk(clk);
    for (int row = 0; row < BATCH_SIZE; row++)
        for (int col = 0; col < INPUT_DIMENSION; col++)
            test_ann_trainer.input_data[row][col](input_data[row][col]);
    for (int row = 0; row < BATCH_SIZE; row++)
        for (int col = 0; col < OUTPUT_DIMENSION; col++)
            test_ann_trainer.input_labels[row][col](input_labels[row][col]);

	// Loading batch data into the arrays and signals
	std::cerr << "Loading data" << std::endl;
	double input_data_raw[BATCH_SIZE][INPUT_DIMENSION];
	double input_labels_raw[BATCH_SIZE][OUTPUT_DIMENSION];
	// Reading the data
	load_data(input_data_raw,input_labels_raw,DATA_FILE_PATH);
	// Sending data to signals
	for (int row = 0; row < BATCH_SIZE; row++)
		for (int col = 0; col < INPUT_DIMENSION; col++)
			input_data[row][col].write(input_data_raw[row][col]);
	for (int row = 0; row < BATCH_SIZE; row++)
		for (int col = 0; col < OUTPUT_DIMENSION; col++)
			input_labels[row][col].write(input_labels_raw[row][col]);

    // Running the module
    test_ann_trainer.initialize();
    sc_start();

    return 0;
}