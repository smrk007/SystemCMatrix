#include "ANN.h"

void feed_forward::set_out1() {
	for (int row = 0; row < 2; row++) {
			for (int col = 0; col < 2; col++) {
				out1[row][col].write(input[row][col].read());
			}
	}
	std::cout << "Output1" << std::endl;
}

void feed_forward::set_out2() {
	for (int row = 0; row < 2; row++) {
			for (int col = 0; col < 2; col++) {
				out2[row][col].write(mult_out1[row][col].read());
			}
	}
	std::cout << "Output2" << std::endl;
}

void feed_forward::set_outf() {
	for (int row = 0; row < 2; row++) {
			for (int col = 0; col < 2; col++) {
				outf[row][col].write(mult_out2[row][col].read());
			}
	}
	std::cout << "Output3" << std::endl;
}
