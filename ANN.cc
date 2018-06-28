#include "ANN.h"

void feed_forward::output() {
	// out1
	for (int row = 0; row < 2; row++) {
			for (int col = 0; col < 2; col++) {
				out1[row][col].write(input[row][col].read());
				out2[row][col].write(mult_out1[row][col].read());
				outf[row][col].write(mult_out2[row][col].read());
			}
	}
	std::cout << "Hello World" << std::endl;
}
