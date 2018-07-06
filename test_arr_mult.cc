#include <iomanip>
#include <iostream>
#include <systemc.h>

#include "SystemCMatrix.h"

#define D1 2
#define D2 2

int sc_main (int argc, char* argv[]) {

    // Signals
    sc_signal<float>    output[D1][D2];

    // Initializing Multiplier 
    ones_array<D1,D2> multiplier("MULTIPLIER");
    for (int row = 0; row < D1; row++) {
        for (int col = 0; col < D2; col++) {
            multiplier.output[row][col](output[row][col]);
        }
    }
    sc_trace_file *trace_file = sc_create_vcd_trace_file("TRACE_FILE");
    for (int row = 0; row < D1; row++) {
        for (int col = 0; col < D2; col++) {
            sc_trace(trace_file, output[row][col], "output");
        }
    }
    sc_start(1,SC_NS);

    // Printing Data
    for (int row = 0; row < D1; row++) {
        for (int col = 0; col < D2; col++) {
            std::cout << output[row][col] << " ";
        }
        std::cout << std::endl;
    }

    return 0;
}