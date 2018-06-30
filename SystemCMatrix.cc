#include "SystemCMatrix.h"

void matrix_adder::add() {
	for (int row = 0; row < 2; row++) {
		for (int col = 0; col < 2; col++) {
			output[row][col].write(input1[row][col].read() + input2[row][col].read());
		}
	}
}

// template <int M1, int S, int M2>
// void matrix_multiplier<M1,S,M2>::multiply() {
// 	clear_temp();
// 	// Filling Temp
// 	for (int i = 0; i < M1; i++) {
// 	for (int j = 0; j < S; j++) {
// 	for (int k = 0; k < M2; k++) {
// 		temp[i][j] += input1[i][k].read() * input2[k][j].read();
// 	}}}
// 	// Sending Temp to Output
// 	for (int row = 0; row < M1; row++) {
// 		for (int col = 0; col < M2; col++) {
// 			output[row][col].write(temp[row][col]);
// 		}
// 	}
// }

// template <int M1, int S, int M2>
// void matrix_multiplier<M1,S,M2>::clear_temp() {
// 	for (int row = 0; row < M1; row++) {
// 		for (int col = 0; col < M2; col++) {
// 			temp[row][col] = 0;
// 		}
// 	}
// }
