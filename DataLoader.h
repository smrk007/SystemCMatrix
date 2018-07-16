#ifndef __DATA_LOADER_H__
#define __DATA_LOADER_H__

#include <cstring>
#include <fstream>
#include <sstream>
#include <string>

#include "Constants.h"

void load_data(float input_data_raw[BATCH_SIZE][INPUT_DIMENSION], float input_labels_raw[BATCH_SIZE][OUTPUT_DIMENSION]) {
    ifstream data_location(DATA_FILE_PATH);
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
                input_data_raw[row][col-1] = atoi(row_element.c_str());
            }
        }
    }
}



#endif /* DataLoader.h */