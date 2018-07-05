#include "SystemCMatrix.h"

void matrix_adder::add() {
	for (int row = 0; row < 2; row++) {
		for (int col = 0; col < 2; col++) {
			output[row][col].write(input1[row][col].read() + input2[row][col].read());
		}
	}
}