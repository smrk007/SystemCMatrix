#ifndef __SPECS_H__
#define __SPECS_H__

// Data specifications
#define BATCH_SIZE 100
#define TEST_BATCH_SIZE 10
#define INPUT_DIMENSION 784
#define HIDDEN_LAYER_DIMENSION 30
#define OUTPUT_DIMENSION 10
#define DATA_FILE_PATH "mnist_data.csv"
#define TEST_DATA_FILE_PATH "mnist_test_data.csv"

// Performance specifications
#define STOP_TIME 10 // Specified in minutes
#define STOP_ITERATION 10
#define STOP_PERCENT 99 // Percent correct
#define STOP_COST 0.0001 // Sum-Error^2
#define STEP_SIZE 0.03

#endif /* Specs.h */