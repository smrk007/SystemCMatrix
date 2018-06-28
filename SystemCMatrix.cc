#include "SystemCMatrix.h"

void matrix_adder::add() {
	for (int row = 0; row < 2; row++) {
		for (int col = 0; col < 2; col++) {
			output[row][col].write(input1[row][col].read() + input2[row][col].read());
		}
	}
}

void matrix_multiplier::multiply() {
	clear_temp();
	// Filling Temp
	for (int i = 0; i < 2; i++) {
	for (int j = 0; j < 2; j++) {
	for (int k = 0; k < 2; k++) {
		temp[i][j] += input1[i][k].read() * input2[k][j].read();
	}}}
	// Sending Temp to Output
	for (int row = 0; row < 2; row++) {
		for (int col = 0; col < 2; col++) {
			output[row][col].write(temp[row][col]);
		}
	}
}

void matrix_multiplier::clear_temp() {
	for (int row = 0; row < 2; row++) {
		for (int col = 0; col < 2; col++) {
			temp[row][col] = 0;
		}
	}
}
