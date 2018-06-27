#include <systemc.h>

SC_MODULE (matrix2x2) {
	// Matrix based on the assumption that all other matrices are 2x2

	// Data input and output
	sc_in<int> input_matrix[2][2];
	sc_out<int> output_matrix;

	// Local data
	int data[2][2];

	// Functions
	void initialize() {

		for (int row = 0; row < 2; row++) {
			for (int col = 0; col < 2; col++) {
				data[row][col] = input_matrix[row][col].read();
			}
		}

	}

	void print() {
		
		for (int row = 0; row < 2; row++) {
			for (int col = 0; col < 2; col++) {
				std::cout << data[row][col] << " ";
			}
			std::cout << std::endl;
		}

	}

	SC_CTOR (matrix2x2) {
		SC_METHOD (initialize);
		SC_METHOD (print);
	}

};
