#ifndef __SPECS_H__
#define __SPECS_H__

// Data specifications
#define BATCH_SIZE 100
#define INPUT_DIMENSION 784
#define HIDDEN_LAYER_DIMENSION 30
#define OUTPUT_DIMENSION 10
#define DATA_FILE_PATH "mnist_data.csv"

// Performance specifications
#define STOP_TIME 10 // Specified in minutes
#define STOP_ITERATION 1000
#define STOP_PERCENT 99 // Percent correct
#define STOP_COST 5 // Sum-Error^2
#define STEP_SIZE 0.03

#endif /* Specs.h */