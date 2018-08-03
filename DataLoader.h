#ifndef __DATA_LOADER_H__
#define __DATA_LOADER_H__

#include <cstring>
#include <fstream>
#include <sstream>
#include <string>

#include "Specs.h"

void load_data(double input_data_raw[BATCH_SIZE][INPUT_DIMENSION], double input_labels_raw[BATCH_SIZE][OUTPUT_DIMENSION], std::string data_file_path) {
    ifstream data_location(data_file_path.c_str());
    for (int row = 0; row < BATCH_SIZE; row++) {

        // Looking at one row
        std::string row_data;
        getline(data_location,row_data);
        std::stringstream row_data_stream(row_data);

        for (int col = 0; col < INPUT_DIMENSION + 1; col++) {
            
            // Looking at one element within a row
            std::string row_element;
            getline(row_data_stream,row_element,',');

            // Dealing with labels
            if (col == 0) {
                input_labels_raw[row][atoi(row_element.c_str())] = 1;
            }
            // Dealing with non-labels
            else {
                input_data_raw[row][col-1] = atof(row_element.c_str()); 
            }
        }
    }
}

void load_test_data(double input_test_data_raw[TEST_BATCH_SIZE][INPUT_DIMENSION], double input_test_labels_raw[TEST_BATCH_SIZE][OUTPUT_DIMENSION], std::string data_file_path) {
    ifstream data_location(data_file_path.c_str());
    for (int row = 0; row < TEST_BATCH_SIZE; row++) {

        // Looking at one row
        std::string row_data;
        getline(data_location,row_data);
        std::stringstream row_data_stream(row_data);

        for (int col = 0; col < INPUT_DIMENSION + 1; col++) {
            
            // Looking at one element within a row
            std::string row_element;
            getline(row_data_stream,row_element,',');

            // Dealing with labels
            if (col == 0) {
                input_test_labels_raw[row][atoi(row_element.c_str())] = 1;
            }
            // Dealing with non-labels
            else {
                input_test_data_raw[row][col-1] = atof(row_element.c_str()); 
            }
        }
    }
}


void load_weights(double weights1_raw[INPUT_DIMENSION][HIDDEN_LAYER_DIMENSION], double weights2_raw[HIDDEN_LAYER_DIMENSION][OUTPUT_DIMENSION]) {

}

#define READ_FROM_CSV(array_name,rows,columns,file_path) \
    do { \
        ifstream fin(file_path); \
        std::cout << "Reading data from: " << file_path << std::endl; \
        for (int row = 0; row < rows; row++) { \
            std::string row_data; \
            getline(fin,row_data); \
            std::stringstream row_data_stream(row_data); \
            for (int col = 0; col < columns; col++) { \
                std::string row_element; \
                getline(row_data_stream,row_element,','); \
                array_name[row][col] = atof(row_element.c_str()); \
            } \
        } \
    } while(0)

#define WRITE_TO_CSV(array_name,rows,columns,file_path) \
    do { \
        ofstream fout(file_path); \
        for (int row = 0; row < rows; row++) { \
            for (int col = 0; col < columns; col++) { \
                fout << array_name[row][col]; \
                if (col < columns - 1) { \
                    fout << ','; \
                } \
            } \
            fout << std::endl; \
        } \
    } while (false);

#endif /* DataLoader.h */